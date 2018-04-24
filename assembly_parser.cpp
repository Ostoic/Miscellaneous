#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <stack>
#include <boost/tokenizer.hpp>

enum class symbol
{
//terminals:
	mov, push, pop, sub, add, call, jmp, nop,
	eax, ecx, edx, ebx, esp, ebp, esi, edi,
	comma,

//nonterminals:
	nullary_op, unary_op, binary_op,
	start_symbol, regstr, instruction, operand,
	eof,
};

class lexer
{
	using tokenizer = boost::tokenizer<boost::char_separator<char>>;

public:
	explicit lexer(const std::string& input) 
		: tokenizer_(input, boost::char_separator<char>())
		, current_token_(tokenizer_.begin())
	{}

	static symbol lex(const std::string& input)
	{ 
		static const std::unordered_map<std::string, symbol> symbols = {
			{"mov", symbol::mov},
			{"push", symbol::push},
			{"pop", symbol::pop},
			{"sub", symbol::sub},
			{"add", symbol::add},
			{"call", symbol::call},
			{"jmp", symbol::jmp},
			{"nop", symbol::nop},

			{"eax", symbol::eax},
			{"ecx", symbol::ecx},
			{"edx", symbol::edx},
			{"ebx", symbol::ebx},
			{"esp", symbol::esp},
			{"ebp", symbol::ebp},
			{"esi", symbol::esi},
			{"edi", symbol::edi},
			{",", symbol::comma},
		};

		if (symbols.count(input) == 0)
			throw std::out_of_range("Invalid input");

		return symbols.at(input);
	}

	bool has_next() const
	{
		return current_token_ != tokenizer_.end();
	}

	symbol lookahead() const
	{
		return lexer::lex(*current_token_);
	}

	void next()
	{
		++current_token_;
	}

private:
	tokenizer tokenizer_;
	tokenizer::iterator current_token_;
};

std::ostream& operator<<(std::ostream& stream, symbol s)
{
	switch (s)
	{
		case symbol::mov: stream << "mov"; break;
		case symbol::eax: stream << "eax"; break;
		case symbol::esp: stream << "esp"; break;
		case symbol::start_symbol: stream << "start_symbol"; break;
		case symbol::binary_op: stream << "binary_op"; break;
		case symbol::regstr: stream << "regstr"; break;
		case symbol::comma: stream << ","; break;
	}

	return stream;
}

using action = void(*)(std::stack<symbol>& productions);

void output_start_symbol(std::stack<symbol>& stack)
{
	stack.pop();
	stack.push(symbol::instruction);
}

template <symbol Register>
void output_register(std::stack<symbol>& stack)
{
	stack.pop();
	stack.push(Register);
}

template <symbol Op>
void output_op(std::stack<symbol>& stack)
{
	stack.pop(); 
	stack.push(Op);
}

namespace std
{
	template <>
	struct hash<std::pair<symbol, symbol>>
	{
		constexpr auto operator()(std::pair<symbol, symbol> s) const
		{
			return hash<int>{}(static_cast<int>(s.first));
		}
	};
}

int main()
{
	const std::unordered_map<std::pair<symbol, symbol>, action> actions = {
		// Output S -> op
		{{symbol::start_symbol, symbol::push}, output_start_symbol },
		{{symbol::start_symbol, symbol::pop},  output_start_symbol },
		{{symbol::start_symbol, symbol::call}, output_start_symbol },
		{{symbol::start_symbol, symbol::jmp},  output_start_symbol },
		{{symbol::start_symbol, symbol::mov},  output_start_symbol },
		{{symbol::start_symbol, symbol::add},  output_start_symbol },
		{{symbol::start_symbol, symbol::sub},  output_start_symbol },
		{{symbol::start_symbol, symbol::nop},  output_start_symbol },

		// Output register -> reg
		{{symbol::regstr, symbol::eax}, output_register<symbol::eax> },
		{{symbol::regstr, symbol::ecx}, output_register<symbol::ecx> },
		{{symbol::regstr, symbol::edx}, output_register<symbol::edx> },
		{{symbol::regstr, symbol::ebx}, output_register<symbol::ebx> },
		{{symbol::regstr, symbol::esp}, output_register<symbol::esp> },
		{{symbol::regstr, symbol::ebp}, output_register<symbol::ebp> },
		{{symbol::regstr, symbol::esi}, output_register<symbol::esi> },
		{{symbol::regstr, symbol::edi}, output_register<symbol::edi> },

		// Output binary_op -> op
		{{symbol::binary_op, symbol::mov}, output_op<symbol::mov> },
		{{symbol::binary_op, symbol::add }, output_op<symbol::add> },
		{{symbol::binary_op, symbol::sub }, output_op<symbol::sub> },

		{{symbol::nullary_op, symbol::nop }, output_op<symbol::nop> },
		{{symbol::unary_op, symbol::call }, output_op<symbol::call> },
		{{symbol::unary_op, symbol::jmp }, output_op<symbol::jmp> },

		// Output instruction -> binary_op register, register
		{{symbol::instruction, symbol::mov}, [](std::stack<symbol>& stack)
		{
			stack.pop();
			stack.push(symbol::regstr);
			stack.push(symbol::comma);
			stack.push(symbol::regstr);
			stack.push(symbol::binary_op);
		}},

		// Output instruction -> unary_op registr
		{{symbol::instruction, symbol::call}, [](std::stack<symbol>& stack)
		{
			stack.pop();
			stack.push(symbol::regstr);
			stack.push(symbol::unary_op);
		}},

		// Output instruction -> unary_op registr
		{{symbol::instruction, symbol::jmp}, [](std::stack<symbol>& stack)
		{
			stack.pop();
			stack.push(symbol::regstr);
			stack.push(symbol::unary_op);
		}},

		// Output instruction -> nullary_op
		{{symbol::instruction, symbol::nop}, [](std::stack<symbol>& stack)
		{
			stack.pop();
			stack.push(symbol::nullary_op);
		}}
	};

	while (true)
	{

		std::string input;
		std::cout << "Enter in x86 assembly to parse: ";
		std::getline(std::cin, input);

		try
		{
			std::stack<symbol> productions;
			productions.push(symbol::start_symbol);

			lexer lexer(input);
			while (lexer.has_next())
			{
				if (productions.empty())
				{
					std::stringstream error_message;
					error_message << "Unexpected symbol " << lexer.lookahead();
					throw std::out_of_range(error_message.str());
				}

				const auto lexeme = std::make_pair(productions.top(), lexer.lookahead());

				// match
				if (lexeme.first == lexeme.second)
				{
					productions.pop();
					lexer.next();
					continue;
				}

				// output production
				if (actions.count(lexeme) == 0)
				{
					std::stringstream error_message;
					error_message 
						<< "Unable to match production state (" << productions.top() 
						<< ") with symbol (" << lexeme.second << ")";
					throw std::out_of_range(error_message.str());
				}

				actions.at(lexeme)(productions);
			}

			if (!productions.empty())
			{
				std::stringstream error_message;
				error_message << "Expected symbol " << productions.top();
				throw std::out_of_range(error_message.str());
			}

			std::cout << "No syntax errors\n\n";
		}
		catch (std::exception& e)
		{
			std::cout << "Syntax error: " << e.what() << "\n\n";
		}
	}

	return 0;
}

#include "ct_map.hpp"

constexpr std::size_t length(const char* string)
{
    std::size_t i = 0;
    for (; string[i] != 0; ++i);
    return i;
}

enum class types
{
    type1, type2, type3, type4
};

// Compiles directly into (or similar)
// return 5 
// Where 5 is the strlen of the m[key]
int main()
{
	constexpr auto m = make_meta_map( 
		std::make_pair(types::type1, "type1"), 
		std::make_pair(types::type2, "type2"),
		std::make_pair(types::type3, "type3"),
		std::make_pair(types::type4, "type4")
	);

    constexpr const auto* name = m[types::type1];
    constexpr auto len = length(name);
	return len;
}
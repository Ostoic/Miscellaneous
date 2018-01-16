#include <array>
#include <tuple>

template <typename Key, typename Value, std::size_t Size = 5>
class ct_map
{
public:
	using key_type = Key;
	using value_type = Value;

	using iterator = typename std::array<std::pair<Key, Value>, Size>::iterator;
	using const_iterator = iterator;

public:
	constexpr ct_map() = default;

	template <typename T, typename... Ts>
	constexpr ct_map(T&& t, Ts&&... ts)
		: data_(std::array<T, 1 + sizeof...(Ts)>{t, std::forward<Ts>(ts)...}) {}

	constexpr iterator begin() const noexcept
	{
		return this->data_.begin();
	}

	constexpr iterator end() const noexcept
	{
		return this->data_.end();
	}

	constexpr std::size_t size() const noexcept
	{
		return this->data_.size();
	}

	constexpr std::size_t count(const key_type& key) const noexcept
	{
		std::size_t count = 0;
        for (std::size_t i = 0; i < Size; i++)
		{
			if (std::get<0>(this->data_[i]) == key)
				count++;
		}

		return count;
	}

	constexpr auto& find(const key_type& key) const
	{
        for (std::size_t i = 0; i < Size; i++)
		{
			if (std::get<0>(this->data_[i]) == key)
				return std::get<1>(this->data_[i]);
		}
		
		// Not found
	}

	constexpr const auto& operator[](const key_type& key) const
	{
		return this->find(key);
	}

private:
	std::array<std::pair<key_type, value_type>, Size> data_;
};

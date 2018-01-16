#include <array>
#include <tuple>

template <typename Key, typename Value, std::size_t Size = 2>
struct map
{
	using key_t = Key;
	using value_t = Value;

	constexpr map() = default;

	template <typename T, typename... Ts>
	constexpr map(T&& t, Ts&&... ts)
		: data_(std::array<T, 1 + sizeof...(Ts)>{t, std::forward<Ts>(ts)...}) {}

	constexpr std::size_t size() const noexcept
	{
		return this->data_.size();
	}

	constexpr std::size_t count() const noexcept
	{
	}

	constexpr const Value& operator[](const Key& key) const noexcept
	{
		for (std::size_t i = 0; i < this->size(); i++)
		{
			if (this->data_[i].first == key)
				return this->data_[i].second;
		}

		//static_assert(false, "Unable to find key");
	}

	std::array<std::pair<Key, Value>, Size> data_;
};

int main()
{
	//std::unordered_map<int, int> x;
	constexpr map<int, const char*> x({ std::make_pair(2, "dank"), std::make_pair(6, "week") });

	std::cout << "2 has: " << x[2];
}
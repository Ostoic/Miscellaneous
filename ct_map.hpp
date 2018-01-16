#pragma once

#include <array>
#include <tuple>

template <typename Key, typename Value, std::size_t Size = 5>
class ct_map
{
public: // subtypes
	using key_type = Key;
	using value_type = Value;

	using iterator = typename std::array<std::pair<Key, Value>, Size>::iterator;
	using const_iterator = iterator;

public: // interface
	constexpr iterator begin() const noexcept;

	constexpr iterator end() const noexcept;

	constexpr std::size_t size() const noexcept;

	constexpr std::size_t count(const key_type& key) const noexcept;

	constexpr auto& find(const key_type& key) const;

	constexpr const auto& operator[](const key_type& key) const;

public: // {ctor}
	constexpr ct_map() = default;

	template <typename T, typename... Ts>
	constexpr ct_map(T&& t, Ts&&... ts);

private: // data
	std::array<std::pair<key_type, value_type>, Size> data_;
};

// Implementation:
#include "ct_map.hxx"
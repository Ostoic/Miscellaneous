#pragma once

#include <array>
#include <tuple>

template <typename Key, typename Value, std::size_t Capacity = 5>
class ct_map
{
public: // subtypes
	using key_type = Key;
	using value_type = Value;

	using iterator = typename std::array<std::pair<Key, Value>, Capacity>::const_iterator;
	using const_iterator = iterator;

public: // interface
	constexpr iterator begin() const noexcept;

	constexpr iterator end() const noexcept;

	constexpr std::size_t size() const noexcept;

	constexpr bool empty() const noexcept;

	constexpr const auto& operator[](const key_type& key) const;

	constexpr std::size_t count(const key_type& key) const noexcept;

	constexpr iterator find(const key_type& key) const;

	//ct_map insert(const std::pair<Key, Value>&) const noexcept;

	//ct_map operator=(const ct_map& other) const noexcept;

public: // {ctor}
	constexpr ct_map() = default;

	constexpr ct_map(const std::array<std::pair<Key, Value>, Capacity>&) noexcept;

	template <typename... Ts>
	constexpr ct_map(Ts&&... ts);

private: // data
	std::array<std::pair<key_type, value_type>, Capacity> data_;
};

template <typename... Ts>
constexpr auto make_meta_map(Ts&&... ts) noexcept;

// Implementation:
#include "meta_map.hxx"
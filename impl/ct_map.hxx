#pragma once
#include "ct_map.hpp"

template <typename K, typename V, std::size_t N = 5>
template <typename T, typename... Ts>
inline constexpr ct_map<K, V, N>::ct_map(T&& t, Ts&&... ts)
	: data_(std::array<T, 1 + sizeof...(Ts)>{t, std::forward<Ts>(ts)...}) {}

template <typename K, typename V, std::size_t N = 5>
inline constexpr typename ct_map<K, V, N>::iterator 
ct_map<K, V, N>::begin() const noexcept
{
	return this->data_.begin();
}

template <typename K, typename V, std::size_t N = 5>
inline constexpr typename ct_map<K, V, N>::iterator 
ct_map<K, V, N>::end() const noexcept
{
	return this->data_.end();
}

template <typename K, typename V, std::size_t N = 5>
inline constexpr std::size_t ct_map<K, V, N>::size() const noexcept
{
	return this->data_.size();
}

template <typename K, typename V, std::size_t N = 5>
inline constexpr std::size_t ct_map<K, V, N>::count(const key_type& key) const noexcept
{
	std::size_t count = 0;
	for (std::size_t i = 0; i < N; i++)
	{
		if (std::get<0>(this->data_[i]) == key)
			count++;
	}

	return count;
}

template <typename K, typename V, std::size_t N = 5>
inline constexpr const auto& ct_map<K, V, N>::find(const key_type& key) const
{
	for (std::size_t i = 0; i < N; i++)
	{
		if (std::get<0>(this->data_[i]) == key)
			return std::get<1>(this->data_[i]);
	}
}

template <typename K, typename V, std::size_t N = 5>
inline constexpr const auto& ct_map<K, V, N>::operator[](const key_type& key) const
{
	return this->find(key);
}

#pragma once

#include <vector>
#include <algorithm>
#include <string_view>

namespace lts
{
	template <typename T, typename Comparator = std::less<T>, typename Allocator = std::allocator<T>>
	class sorted_vector
	{
	public:
		using iterator = typename std::vector<T>::iterator;
		using const_iterator = typename std::vector<T>::const_iterator;

	public:
		sorted_vector() = default;

		explicit sorted_vector(std::size_t size, Comparator c = std::less<T>()) : m_data(size) {}

		const T& operator[](std::size_t i) const
		{
			return m_data[i];
		}

		T& operator[](std::size_t i)
		{
			m_sorted = false;
			return m_data[i];
		}

		void insert(T&& value)
		{
			m_data[m_index++] = std::move(value);
			m_sorted = false;
		}

		iterator find(const T& key)
		{
			return this->find(key, begin());
		}

		iterator find(const T& key, iterator start)
		{
			if (!m_sorted) this->sort();
			const auto it = std::lower_bound(start, m_data.end(), key, m_compare);
			return it;
		}

		std::vector<std::string_view> find_matches(const T& key, std::size_t size)
		{
			std::vector<std::string_view> matches;
			matches.reserve(size);

			auto currentIt = begin();

			for (int x = 0; x < size; x++, ++currentIt)
			{
				currentIt = find(key, currentIt);
				if (currentIt == end())
					break;

				// Make sure we have a real partial match
				std::string_view potential = *currentIt;
				if (key == potential.substr(0, key.size()))
					matches.push_back(potential);
			}

			return matches;
		}

		iterator begin()
		{ return m_data.begin(); }

		iterator end()
		{ return m_data.end(); }

		const_iterator begin() const
		{ return m_data.begin(); }

		const_iterator end() const
		{ return m_data.end(); }

		void sort()
		{
			std::sort(m_data.begin(), m_data.end(), m_compare);
			m_sorted = true;
		}

	private:

		bool m_sorted = false;
		std::vector<T, Allocator> m_data;
		std::size_t m_index = 0;
		Comparator m_compare;
	};
}

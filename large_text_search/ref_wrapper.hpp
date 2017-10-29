#pragma once

namespace lts
{
	template<class T>
	class ref_wrapper
	{
	public:
		ref_wrapper() : m_value(nullptr) {}

		ref_wrapper(T& value) : m_value(&value) {}

		operator const T&() const
		{ return *m_value; }

		operator T&()
		{ return *m_value; }

		ref_wrapper<T>& operator=(const T& value)
		{
			*m_value = value;
			return *this;
		}

	private:
		T* m_value;
	};
}


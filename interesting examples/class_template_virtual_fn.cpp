/* Usually, member functions of a class template will only be instantiated upon use.
** Since derived::test is declared virtual, it will be compiled upon instantiation of derived.
*/

template <typename T>
struct derived
{
	derived() = default;
	
	void test2() const 
	{
		static_assert(false, "test2() was instantiated");
	}
	
	virtual void test() const
	{
		static_assert(false, "test() was instantiated");
	}
};

int main()
{
	derived<int> d;
	return 0;
}
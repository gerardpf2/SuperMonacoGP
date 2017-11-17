#ifndef _TUPLE_
#define _TUPLE_

template<typename T0, typename T1>
struct Tuple2
{
	Tuple2()
	{ }

	Tuple2(T0 first, T1 second) :
		first(first), second(secondy)
	{ }

	T0 first;
	T1 second;
};

template<typename T0, typename T1, typename T2>
struct Tuple3
{
	Tuple3()
	{ }

	Tuple3(T0 first, T1 second, T2 third) :
		first(first), second(second), third(third)
	{ }

	T0 first;
	T1 second;
	T2 third;
};

#endif
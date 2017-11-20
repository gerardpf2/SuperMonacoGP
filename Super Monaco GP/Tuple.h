#ifndef _TUPLE_
#define _TUPLE_

template<typename T0, typename T1>
struct Tuple2
{
	Tuple2()
	{ }

	Tuple2(T0 first, T1 second) :
		first(first), second(second)
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

template<typename T0, typename T1, typename T2, typename T3>
struct Tuple4
{
	Tuple4()
	{ }

	Tuple4(T0 first, T1 second, T2 third, T3 fourth) :
		first(first), second(second), third(third), fourth(fourth)
	{ }

	T0 first;
	T1 second;
	T2 third;
	T3 fourth;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4>
struct Tuple5
{
	Tuple5()
	{ }

	Tuple5(T0 first, T1 second, T2 third, T3 fourth, T4 fifth) :
		first(first), second(second), third(third), fourth(fourth), fifth(fifth)
	{ }

	T0 first;
	T1 second;
	T2 third;
	T3 fourth;
	T4 fifth;
};

#endif
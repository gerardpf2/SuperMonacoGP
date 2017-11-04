#ifndef _TUPLE_
#define _TUPLE_

template<typename T>
struct Tuple2
{
	Tuple2()
	{ }

	Tuple2(T x, T y) :
		x(x), y(y)
	{ }

	T x, y;
};

template<typename T>
struct Tuple3
{
	Tuple3()
	{ }

	Tuple3(T x, T y, T z) :
		x(x), y(y), z(z)
	{ }

	T x, y, z;
};

#endif
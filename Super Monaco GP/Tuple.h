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
struct Tuple3 : public Tuple2<T>
{
	Tuple3() :
		Tuple2()
	{ }

	Tuple3(T x, T y, T z) :
		Tuple2(x, y), z(z)
	{ }

	T z;
};

#endif
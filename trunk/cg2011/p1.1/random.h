#ifndef RANDOM_H
#define RANDOM_H

class RandomInput
{
	int seed;

	static const int a = 1664525;
	static const int c = 1013904223;
public:
	RandomInput(int seed)
		 : seed(seed)
	{ }

	// returns random integer in range 0 .. 2^31-1
	int get_int()
	{
		 return (seed = seed * a + c) & 0x7fffffff;
	}

	double get_double()
	{
		 const double lo = -1000;
		 const double hi = 1000;
		 const double scale = 1. / (1 << 30);
		 return (lo + (hi - lo) * scale * (get_int() & 0x3fffffff));
	}
};

#endif

#include "io.h"

// RandomInput implementation starts

io::RandomInput::RandomInput(int seed) : seed(seed)
{ }

int io::RandomInput::get_int()
{
	return (seed = seed * a + c) & 0x7fffffff;
}

double io::RandomInput::get_double()
{
	const double lo = -1000;
	const double hi = 1000;
	const double scale = 1. / (1 << 30);
	return (lo + (hi - lo) * scale * (get_int() & 0x3fffffff));
}

// RandomInput implementation ends

// StreamInput implementation starts

io::StreamInput::StreamInput(std::istream& stream) : stream(stream)
{ }

int io::StreamInput::get_int()
{
	int result;
	stream >> result;
	return result;
}

double io::StreamInput::get_double()
{
	double result;
	stream >> result;
	return result;
}

// StreamInput implementation ends

// HashOutput implementation starts



// HashOutput implementation ends

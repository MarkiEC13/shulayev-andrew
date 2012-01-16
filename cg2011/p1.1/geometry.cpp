#include <fstream>

#include "random.h"

struct point
{
	double x, y;
	point() { }
	point(double x, double y) : x(x), y(y) { }
	point(RandomInput& rand) : x(rand.get_double()), y(rand.get_double()) { }
};

std::istream& operator>>(std::istream& in, point& p)
{
	in >> p.x >> p.y;
}

std::ostream& operator<<(std::ostream& out, const point& p)
{
	out << '(' << p.x << "; " << p.y << ')';
}

struct segment
{
	point end[2];
	segment() { }
	segment(RandomInput& rand)
	{
		end[0] = point(rand);
		end[1] = point(rand);
	}
};

std::istream& operator>>(std::istream& in, segment& s)
{
	in >> s.end[0] >> s.end[1];
}

std::ostream& operator<<(std::ostream& out, const segment& s)
{
	out << '(' << s.end[0] << ", " << s.end[1] << ')';
}

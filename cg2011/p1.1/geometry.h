#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <fstream>

#include "random.h"

struct point
{
	double x, y;
	point();
	point(double, double);
	point(RandomInput&);
};

std::ostream& operator<<(std::ostream&, const point&);
std::istream& operator>>(std::istream&, point&);

struct segment
{
	point end[2];
	segment();
	segment(point, point);
	segment(RandomInput&);
};

std::ostream& operator<<(std::ostream&, const segment&);
std::istream& operator>>(std::istream&, segment&);

#endif

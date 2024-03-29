#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <limits>
#include <fstream>
#include <boost/numeric/interval.hpp>

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
	point a, b;
	segment();
	segment(point, point);
	segment(RandomInput&);
};

std::ostream& operator<<(std::ostream&, const segment&);
std::istream& operator>>(std::istream&, segment&);

bool segments_intersect(const segment&, const segment&);

int left_turn(const segment&, const point& p);

int left_turn_simple(const segment&, const point&);
int left_turn_interval(const segment&, const point&);
int left_turn_adaptive(const segment&, const point&);

#endif

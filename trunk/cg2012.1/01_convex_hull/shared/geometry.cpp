#include <cassert>

#include "geometry.h"

// Point implementation starts

geometry::Point::Point() : x(0.0), y(0.0)
{ }

geometry::Point::Point(double x, double y) : x(x), y(y)
{ }

geometry::Point::Point(io::Input & input)
{
	x = input.get_double();
	y = input.get_double();
}

double geometry::Point::get_x() const
{
	return x;
}

double geometry::Point::get_y() const
{
	return y;
}

// Point implementation ends

// Segment implementation starts

geometry::Segment::Segment(const geometry::Point & end0, const geometry::Point & end1)
{
	ends[0] = end0;
	ends[1] = end1;
}

geometry::Segment::Segment(io::Input & input)
{
	ends[0] = Point(input);
	ends[1] = Point(input);
}

geometry::Point geometry::Segment::get_end(int index) const
{
	assert(index == 0 || index == 1);

	return ends[0];
}

bool geometry::Segment::does_intersect(const geometry::Segment &) const
{
	return false;
}

// Segment implementation ends

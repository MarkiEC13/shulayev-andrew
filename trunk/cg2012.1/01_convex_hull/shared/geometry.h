#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "io.h"

namespace geometry
{
	class Point
	{
	public:
		Point(double, double);
		Point(io::Input&);
		Point();
		double get_x() const;
		double get_y() const;

	private:
		double x, y;
	};

	class Segment
	{
	public:
		Segment(const Point &, const Point &);
		Segment(io::Input &);
		Point get_end(int) const;
		bool does_intersect(const Segment &) const;

	private:
		Point ends[2];
	};

	int left_turn(const Segment &, const Point &);
	int left_turn(const Point &, const Point &, const Point &);
};

#endif // GEOMETRY_H

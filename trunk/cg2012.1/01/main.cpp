#include <iostream>
#include <memory>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cassert>
#include <stack>
#include <fstream>

#include "shared/io.h"
#include "shared/geometry.h"

struct comparator
{
	geometry::Point * start;

	comparator(geometry::Point * start) : start(start)
	{ }

	int get_turn(const geometry::Point & p1, const geometry::Point & p2)
	{
		return geometry::left_turn(*start, p1, p2);
	}

	bool operator()(const geometry::Point & p1, const geometry::Point & p2)
	{
		int turn = get_turn(p1, p2);
		if (turn == 0)
		{
			if (p1.get_y() == p2.get_y())
			{
				return fabs(start->get_x() - p1.get_x()) < fabs(start->get_x() - p2.get_x());
			}
			else
			{
				return p1.get_y() < p2.get_y();
			}
		}
		else
		{
			return turn == 1;
		}
	}
};

inline bool is_lower(const geometry::Point & p1, const geometry::Point & p2)
{
	return p1.get_y() > p2.get_y() || (p1.get_y() == p2.get_y() && p1.get_x() > p2.get_x());
}

int main()
{
	io::StreamInput in(std::cin);
	int count = in.get_int();

	std::vector<geometry::Point> points(count);
	for (int i = 0; i < count; ++i)
	{
		points[i] = geometry::Point(in);
	}

	size_t size = geometry::convex_hull(points);

	std::cout << size << '\n';
	for (size_t i = 0; i < size; ++i)
	{		
		std::cout << points[i].get_x() << ' ' << points[i].get_y() << '\n';
	}

	return 0;
}

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
	int selected = 0;
	for (int i = 0; i < count; ++i)
	{
		points[i] = geometry::Point(in);
		if (i == 0 || is_lower(points[selected], points[i]))
		{
			selected = i;
		}
	}

	std::swap(points[selected], points[0]);
	std::sort(points.begin() + 1, points.end(), comparator(&points[0]));

	if (points.size() < 3)
	{
		std::cout << points.size() << '\n';
		for (size_t i = 0; i < points.size(); ++i)
		{
			std::cout << points[i].get_x() << ' ' << points[i].get_y() << '\n';
		}
		return 0;
	}

	std::vector<geometry::Point> stack;
	stack.push_back(points[0]);
	stack.push_back(points[1]);
	stack.push_back(points[2]);
	for (size_t i = 3; i < points.size(); ++i)
	{
		while (stack.size() > 2 && geometry::left_turn(stack[stack.size() - 2], stack[stack.size() - 1], points[i]) != 1)
		{
			stack.pop_back();
		}
		stack.push_back(points[i]);
	}

	std::cout << stack.size() << '\n';
	for (size_t i = 0; i < stack.size(); ++i)
	{		
		std::cout << stack[i].get_x() << ' ' << stack[i].get_y() << '\n';
	}

	return 0;
}

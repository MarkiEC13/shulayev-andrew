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

namespace comparator
{
	std::pair<geometry::Point, int> start;

	int get_turn(const geometry::Point & p1, const geometry::Point & p2)
	{
		return geometry::left_turn(start.first, p1, p2);
	}

	bool less1(const std::pair<geometry::Point, int>& p1, const std::pair<geometry::Point, int>& p2)
	{
		int turn = get_turn(p1.first, p2.first);
		if (turn == 0)
		{
			// std::cerr << p1.first.get_x() << ';' << p1.first.get_y() << ' ';
			// std::cerr << p2.first.get_x() << ';' << p2.first.get_y() << '\n';
			if (p1.first.get_y() == p2.first.get_y())
			{
				return fabs(start.first.get_x() - p1.first.get_x()) < fabs(start.first.get_x() - p2.first.get_x());
			}
			else
			{
				return p1.first.get_y() < p2.first.get_y();
			}
		}
		else
		{
			return turn == 1;
		}
	}
};

int main()
{
	io::StreamInput in(std::cin);
	int count = in.get_int();

	if (count < 3)
	{
		std::cout << count << '\n';
		for (int i = 0; i < count; ++i)
		{
			if (i > 0)
			{
				std::cout << ' ';
			}
			std::cout << (i + 1);
		}
		std::cout << '\n';
	}

	std::vector<std::pair<geometry::Point, int> > points(count);
	int selected = 0;
	for (int i = 0; i < count; ++i)
	{
		points[i] = std::make_pair(geometry::Point(in), i);
		if (i == 0 || comparator::start.first.get_y() > points[i].first.get_y()
			|| (comparator::start.first.get_y() == points[i].first.get_y()
				&& comparator::start.first.get_x() > points[i].first.get_x()))
		{
			comparator::start = points[i];
			selected = i;
		}
	}

	// std::cerr << "start: " << comparator::start.first.get_x() << ';' << comparator::start.first.get_y() << '\n';
	std::swap(points[selected], points[count - 1]);
	points.pop_back();

	std::sort(points.begin(), points.end(), &comparator::less1);

	std::vector<std::pair<geometry::Point, int> > filtered_points;
	for (int i = 0; i < count - 2; ++i) {
		if (comparator::get_turn(points[i].first, points[i + 1].first) != 0)
		{
			filtered_points.push_back(points[i]);
		}
	}
	filtered_points.push_back(points[count - 2]);

	if (filtered_points.size() < 3)
	{
		std::cout << filtered_points.size() + 1 << '\n' << comparator::start.second + 1;
		for (size_t i = 0; i < filtered_points.size(); ++i)
		{
			std::cout << ' ' << filtered_points[i].second + 1;
		}
		std::cout << '\n';
		return 0;
	}

	std::vector<std::pair<geometry::Point, int> > stack;
	stack.push_back(comparator::start);
	stack.push_back(filtered_points[0]);
	stack.push_back(filtered_points[1]);
	for (size_t i = 2; i < filtered_points.size(); ++i)
	{
		while (stack.size() > 2 && geometry::left_turn(stack[stack.size() - 2].first, stack[stack.size() - 1].first, filtered_points[i].first) < 0)
		{
			stack.pop_back();
		}
		stack.push_back(filtered_points[i]);
	}

	std::cout << stack.size() << '\n';
	for (size_t i = 0; i < stack.size(); ++i)
	{		
		if (i > 0)
		{
			std::cout << ' ';
		}
		std::cout << stack[i].second + 1;
	}
	std::cout << '\n';

	return 0;
}

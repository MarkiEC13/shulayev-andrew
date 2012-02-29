#include <iostream>
#include <memory>
#include <algorithm>
#include <cmath>
#include <vector>

#include "shared/io.h"
#include "shared/geometry.h"

geometry::Point start;

bool less1(const geometry::Point& p1, const geometry::Point& p2)
{
	int turn = geometry::left_turn(start, p1, p2);
	if (turn == 0)
	{
		if (p1.get_y() == p2.get_y())
		{
			return fabs(start.get_x() - p1.get_x()) < fabs(start.get_x() - p2.get_x());
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

int main()
{
	io::StreamInput in(std::cin);
	int count = in.get_int();

	std::vector<geometry::Point> points(count);
	int selected = 0;
	for (int i = 0; i < count; ++i)
	{
		points[i] = geometry::Point(in);
		if (i == 0 || start.get_y() > points[i].get_y())
		{
			start = points[i];
			selected = i;
		}
	}
	std::swap(points[selected], points[count - 1]);
	points.pop_back();

	std::sort(points.begin(), points.end(), less1);

	for (int i = 0; i < count - 1; ++i)
	{
		std::cout << points[i].get_x() << ' ' << points[i].get_y() << '\n';
	}

	return 0;
}

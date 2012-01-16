#include <iostream>

#include "random.h"
#include "geometry.h"

int main()
{
	int n, type;
	std::cin >> n >> type;
	
	int p = 1, result = 0;
	if (type == 0)
	{
		segment seg1, seg2;
		for (int i = 0; i < n; i++, p *= 239)
		{
			std::cin >> seg1 >> seg2;
		}
	}
	else
	{
		int seed;
		std::cin >> seed;
		RandomInput rand(seed);
		
		for (int i = 0; i < n; i++, p *= 239)
		{
			segment seg1(rand);
			segment seg2(rand);
		}
	}
	
	std::cout << result;
	
	return 0;
}

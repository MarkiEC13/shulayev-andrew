// problem_2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

double get_area(std::vector< double >& x, std::vector< double >& y)
{
   double or_area = 0.0;
   
   x.push_back(x[0]);
   y.push_back(y[0]);
   
   for (int i = 0; i < x.size() - 1; i++)
   {
      or_area += x[i + 1] * y[i] - x[i] * y[i + 1];
   }
   
   x.pop_back();
   y.pop_back();
   
   return fabs(or_area * 0.5);
}

int main()
{
   std::ifstream input("in.txt");
   std::ofstream output("out.txt");
   
   std::vector< double > x, y;
   double cx, cy;
   
   while (input >> cx >> cy)
   {
      x.push_back(cx);
      y.push_back(cy);
   }
   
   double ans = get_area(x, y);
   output << std::setprecision(6) << ans << std::endl;
   
   return 0;
}
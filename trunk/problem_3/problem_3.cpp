// problem_3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int gcd_extended(int a, int b, int& x, int& y)
{
   if (a == 0)
   {
      x = 0; y = 1;
      return b;
   }
   int x1, y1;
   int d = gcd_extended(b % a, a, x1, y1);
   x = y1 - (b / a) * x1;
   y = x1;
   
   return d;
}

int main()
{
   std::ifstream input("in.txt");
   std::ofstream output("out.txt");
   
   int A, B, C;
   int x, y, g;
   while (input >> A >> B >> C)
   {
      g = gcd_extended(A, B, x, y);
      if (C % g != 0) output << "<none>" << std::endl;
      else
      {
         x *= C / g;
         y *= C / g;
         output << x << ' ' << y << std::endl;
      }
   }
   
   return 0;
}


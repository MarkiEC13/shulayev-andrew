#include "bignum.h"

int main()
{
   std::ifstream in("in.txt");
   std::ofstream out("out.txt");

   bignum x, y;

   while (in >> x >> y)
   {
      if (y.is_zero())
      {
         out << "<error>\n\n\n";
      }
      else
      {
         std::pair<bignum, bignum> result = x.divide(y);
         out << result.first << "\n";
         out << result.second << "\n\n";
      }
   }

   return 0;
}
#include <fstream>

typedef long long int64;

int64 gcd_extended(int64 a, int64 b, int64& x, int64& y)
{
   if (a == 0)
   {
      x = 0; y = 1;
      return b;
   }
   int64 x1, y1;
   int64 d = gcd_extended(b % a, a, x1, y1);
   x = y1 - (b / a) * x1;
   y = x1;
   
   return d;
}

int main()
{
   std::ifstream input("in.txt");
   std::ofstream output("out.txt");
   
   int64 a, b, c;
   int64 x, y, g;
   while (input >> a >> b >> c)
   {
      if (a == 0 && b == 0)
      {
         if (c == 0) output << "1 1\n";
         else output << "<none>\n";
      }
      else if (a == 0)
      {
         if (c % b == 0) output << "1 " << c / b << '\n';
         else output << "<none>\n";
      }
      else if (B == 0)
      {
         if (c % a == 0) output << c / a << " 1\n";
         else output << "<none>\n";
      }
      else
      {
         g = gcd_extended(a, b, x, y);
         if (a % g != 0) output << "<none>\n";
         else
         {
            x *= c / g;
            y *= c / g;
            output << x << ' ' << y << '\n';
         }
      }
   }
   
   return 0;
}

#include <fstream>

typedef long long int64;

// инвариант: ax + by = gcd(a, b)
int gcd_extended(int a, int b, int64& x, int64& y)
{
   if (b == 0)
   {
      x = 1;
      y = 0;

      return a;
   }

   int gcd = gcd_extended(b, a % b, x, y);
   // bx + (a % b)y = gcd(a, b)

   int64 temp = x;
   x = y - (b / a) * x;
   y = temp;

   return gcd;
}

int main()
{
   std::ifstream in("in.txt");
   std::ofstream out("out.txt");

   int a, b, c, gcd;
   int64 x, y;
   while (in >> a >> b >> c)
   {
      if (a == 0 && b == 0)
      {
         if (c == 0) 
         {
            out << "1 1\n";
         }
         else
         {
            out << "<none>\n";
         }
      }

      else
      {
         gcd = gcd_extended(a, b, x, y);

         if (c % gcd == 0)
         {
            out << x * (c / gcd) << ' ' << y * (c / gcd) << '\n';
         }
         else
         {
            out << "<none>\n";
         }
      }
   }

   return 0;
}
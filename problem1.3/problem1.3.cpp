#include <fstream>

typedef long long int64;

// èíâàðèàíò: ax + by = gcd(a, b)
int gcd_extended(int a, int b, int64& x, int64& y)
{
   if (a == 0)
   {
      x = 0;
      y = 1;

      return b;
   }

   int gcd = gcd_extended(b % a, a, x, y);

   int64 temp = x;
   x = y - (b / a) * x;
   y = temp;

   return gcd;
}

int main()
{
   std::ifstream in("in.txt");
   std::ofstream out("out.txt");

   int a, b, c;
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
         int gcd;
         int64 x, y;
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
#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>

double get_area(std::vector< double >& x, std::vector< double >& y)
{
   double or_area = 0.0;
   int n = x.size();
   if (n == 0) return 0.0;
   
   for (size_t i = 0; i < x.size(); i++)
   {
      or_area += x[(i + 1) % n] * y[i % n] - x[i % n] * y[(i + 1) % n];
   }
   
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
   output << std::setprecision(6) << ans << '\n';
   
   return 0;
}

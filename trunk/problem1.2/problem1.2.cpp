#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>

double get_area(const std::vector< double >& x, const std::vector< double >& y)
{
   double area = 0.0;
   size_t n = x.size();
   if (n == 0)
   {
      return 0;
   }
   
   for (size_t i = 0; i < n; i++)
   {
      area += x[(i + 1) % n] * y[i % n] - x[i % n] * y[(i + 1) % n];
   }
   
   return fabs(area * 0.5);
}

int main()
{
   std::ifstream input("in.txt");
   std::ofstream output("out.txt");
   
   std::vector< double > x, y;
   double coord_x, coord_y;
   
   while (input >> coord_x >> coord_y)
   {
      x.push_back(coord_x);
      y.push_back(coord_y);
   }
   
   double ans = get_area(x, y);
   output << std::fixed << std::setprecision(8) << ans << '\n';
   
   return 0;
}

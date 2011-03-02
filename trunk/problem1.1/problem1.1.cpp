#include <fstream>

const int DIRECTIONS = 4;

void show_spiral(int size, int x, int y, std::ofstream& out)
{
   int dx[] = { 1, 0, -1, 0 };
   int dy[] = { 0, 1, 0, -1 };
   
   int curr_size = 1;
   int curr_point = 0;
   int curr_direction = 0;
   bool increase_next = false;
   
   for (int i = 0; i < size; i++)
   {
      out << x << ' ' << y << '\n';
      
      x += dx[curr_direction] * curr_size;
      y += dy[curr_direction] * curr_size;
      
      curr_direction++;
      curr_point = 0;
      
      if (curr_direction == DIRECTIONS) curr_direction = 0;
      
      if (increase_next)
      {
         curr_size++;
         increase_next = false;
      }
      else increase_next = true;
   }
}

int main()
{
   std::ifstream input("in.txt");
   std::ofstream output("out.txt");
   
   int n;
   input >> n;
   
   show_spiral(n, 0, 0, output);
   
   return 0;
}

#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using std::cerr;

struct big
{
   int size;
   std::vector< int > data;
   big();
   big(std::string);
   big operator-(big);
   void out_big(std::ostream&);
   void add_digit(int);
   big operator*(int);
   std::pair< big, big > operator/(big);
   bool operator<(big);
};

big::big()
{
   size = 0;
   data = std::vector< int >();
}

big::big(std::string s)
{
   data.resize(s.length());
   int i = 0;
   size = 0;
   
   while (i < s.length() && s[i] == '0') i++;
   
   while ((int)s.length() - size - 1 >= i)
   {
      data[size] = s[s.length() - size - 1] - '0';
      size++;
   }
}

big big::operator-(big b)
{
   bool prev = false;
   big result = big();
   for (int i = 0; i < size; i++)
   {
      if (i < b.size) result.data.push_back(data[i] - b.data[i]);
      else result.data.push_back(data[i]);
      
      if (prev)
      {
         result.data[i]--;
      }
      prev = false;
      
      if (result.data[i] < 0)
      {
         result.data[i] += 10;
         prev = true;
      }
      
      if (result.data[i] != 0)
      {
         result.size = i + 1;
      }
   }
   return result;
}

big big::operator*(int m)
{
   big result;
   
   int carry = 0;
   for (int i = 0; i < size; i++)
   {
      result.data.push_back(data[i] * m + carry);
      carry = result.data[i] / 10;
      result.data[i] %= 10;
      
      if (result.data[i] != 0) result.size = i + 1;
   }
   while (carry > 0)
   {
      result.data.push_back(carry % 10);
      carry /= 10;
      result.size = result.data.size();
   }
   
   return result;
}

void big::out_big(std::ostream& out)
{
   for (int i = size - 1; i >= 0; i--)
   {
      out << data[i];
   }
   if (size == 0) out << '0';
   out << '\n';
}

void big::add_digit(int digit)
{
   std::vector< int > new_data = std::vector< int >(size + 1);
   for (int i = 0; i < size; i++)
   {
      new_data[i + 1] = data[i];
   }
   new_data[0] = digit;
   data = new_data;
   if (!(size == 0 && digit == 0)) size++;
}

bool big::operator<(big o)
{
   if (size != o.size)
   {
      return size < o.size;
   }
   else for (int i = size - 1; i >= 0; i--)
   {
      if (data[i] != o.data[i])
      {
         return data[i] < o.data[i];
      }
   }
   return true;
}

std::pair< big, big > big::operator/(big o)
{
   big curr;
   big result;
   
   bool started = false;
   for (int i = size - 1; i >= 0; i--)
   {
      curr.add_digit(data[i]);
      if (o < curr)
      {
         int i = 0;
         while ((o * (i + 1)) < curr)
         {
            i++;
         }
         result.add_digit(i);
         curr = curr - o * i;
         started = true;
      }
      else if (started)
      {
         result.add_digit(0);
      }
   }
   return std::make_pair(result, curr);
}

int main()
{
   std::ifstream input("in.txt");
   std::ofstream output("out.txt");
   
   std::string a, b;
   
   while (input >> a >> b)
   {
      big A = big(a);
      big B = big(b);
      if (B.size == 0)
      {
         output << "<error>\n\n";
      }
      else
      {
         std::pair< big, big > result = A / B;
         result.first.out_big(output);
         result.second.out_big(output);
      }
      output << '\n';
   }
   
   return 0;
}

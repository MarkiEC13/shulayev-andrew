#include "big_int.h"

// containers

big_int::big_int()
{
   is_negative = false;
   digits.push_back(0);
}

big_int::big_int(int64 data)
{
   is_negative = (data < 0);
   data = abs(data);

   while (data > 0)
   {
      digits.push_back(data % BASE);
      data /= BASE;
   }

   if (digits.size() == 0)
   {
      digits.push_back(0);
   }
}

big_int::big_int(int data)
{
   big_int(int64(data));
}

// input/output routines

std::istream& operator>>(std::istream& in, big_int& big)
{
   if (in.peek() == '-')
   {
      in.get();
      big.is_negative = true;
   }

   std::vector<char> buffer;
   while ('0' <= in.peek() && in.peek() <= '9')
   {
      if (in.peek() != '0' || !buffer.empty())
      {
         buffer.push_back(in.get());
      }
      else
      {
         in.get();
      }
   }

   container_t new_digits;

   if (buffer.size() == 0)
   {
      new_digits.push_back(0);
      big.is_negative = false;
   }
   else
   {
      int read_digits = 0, current = 0;

      int badness = buffer.size() % LOG10_BASE;
      int size = buffer.size() / LOG10_BASE + 1;
      if (badness == 0) size--;

      new_digits.ensure_capacity(size);

      for (size_t i = 0; i < buffer.size(); i++)
      {
         if (i % LOG10_BASE == badness && read_digits != 0)
         {
            new_digits.push_back(current);
            current = 0;
            read_digits = 0;
         }

         current = current * 10 + buffer[i] - '0';
         read_digits++;
      }
      if (read_digits != 0)
      {
         new_digits.push_back(current);
      }

      new_digits.reverse();
   }
   big.digits = new_digits;

   return in;
}

std::ostream& operator<<(std::ostream& out, const big_int& big)
{
   if (big.is_negative)
   {
      out << '-';
   }
   
   out << big.digits[big.digits.size() - 1];

   for (int i = big.digits.size() - 2; i >= 0; i--)
   {
      out << std::setw(LOG10_BASE) << std::setfill('0') << big.digits[i];
   }

   return out;
}

// comparison operators

int big_int::compare_to(const big_int& other) const
{
   if (digits.size() != other.digits.size())
   {
      return digits.size() - other.digits.size();
   }

   for (int i = digits.size() - 1; i >= 0; i--)
   {
      if (digits[i] != other.digits[i])
      {
         return digits[i] - other.digits[i];
      }
   }

   return 0;
}

bool big_int::operator==(const big_int& other) const
{
   return (compare_to(other) == 0);
}

bool big_int::operator<=(const big_int& other) const
{
   return (compare_to(other) <= 0);
}

bool big_int::operator>=(const big_int& other) const
{
   return (compare_to(other) >= 0);
}

bool big_int::operator!=(const big_int& other) const
{
   return (compare_to(other) != 0);
}

bool big_int::operator<(const big_int& other) const
{
   return (compare_to(other) < 0);
}

bool big_int::operator>(const big_int& other) const
{
   return (compare_to(other) > 0);
}

// assignment operator

big_int& big_int::operator=(const big_int& other)
{
   is_negative = other.is_negative;
   digits = other.digits;

   return *this;
}

// swap method

void big_int::swap(big_int& other)
{
   digits.swap(other.digits);
   std::swap(is_negative, other.is_negative);
}
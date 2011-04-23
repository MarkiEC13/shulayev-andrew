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
   while (in.peek() == ' ' || in.peek() == '\n' || in.peek() == '\t')
   {
      in.get();
   }

   big.is_negative = false;
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

int big_int::compare_to(const big_int& other, size_t shift = 0) const
{
   if (digits.size() - shift != other.digits.size())
   {
      return digits.size() - shift - other.digits.size();
   }

   for (int i = other.digits.size() - 1; i >= 0; i--)
   {
      if (digits[shift + i] != other.digits[i])
      {
         return digits[shift + i] - other.digits[i];
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

bool big_int::is_zero() const
{
   return (digits.size() == 1 && digits[0] == 0);
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

// additive inverse

big_int big_int::operator-() const
{
   big_int result = *this;
   
   if (!result.is_zero())
   {
      result.is_negative = !result.is_negative;
   }
   return result;
}

// arithmetic routines

big_int& big_int::operator+=(const big_int& other)
{
   if (is_negative != other.is_negative)
   {
      return *this -= -other;
   }

   int carry = 0;

   size_t i;
   for (i = 0; i < other.digits.size(); i++)
   {
      if (i == digits.size())
      {
         digits.push_back(0);
      }

      digits[i] += carry + other.digits[i];
      carry = digits[i] / BASE;
      digits[i] %= BASE;
   }

   while (carry > 0)
   {
      if (i == digits.size())
      {
         digits.push_back(0);
      }

      digits[i] += carry;
      carry = digits[i] / BASE;
      digits[i] %= BASE;

      i++;
   }

   return *this;
}

big_int big_int::operator+(const big_int& other) const
{
   return big_int(*this) += other;
}

big_int& big_int::operator-=(const big_int& other)
{
   if (is_negative != other.is_negative)
   {
      return *this += -other;
   }

   int carry = 0;

   size_t i;
   for (i = 0; i < other.digits.size(); i++)
   {
      if (i == digits.size())
      {
         digits.push_back(0);
      }

      digits[i] -= carry + other.digits[i];

      carry = 0;
      if (digits[i] < 0)
      {
         digits[i] += BASE;
         carry = 1;
      }
   }

   while (i < digits.size() && carry != 0)
   {
      digits[i] -= carry;
      
      carry = 0;
      if (digits[i] < 0)
      {
         digits[i] += BASE;
         carry = 1;
      }

      i++;
   }

   if (carry != 0)
   {
      is_negative = !is_negative;

      i = 0;
      while (i < digits.size() && digits[i] == 0)
      {
         i++;
      }

      digits[i] = BASE - digits[i];
      i++;

      while (i < digits.size())
      {
         digits[i] = BASE - digits[i] - 1;
         i++;
      }
   }

   normalize();

   return *this;
}

big_int big_int::operator-(const big_int& other) const
{
   return big_int(*this) -= other;
}

big_int big_int::operator*(const big_int& other) const
{
   container_t result;

   int64 carry = 0;
   for (size_t i = 0; i < digits.size(); i++)
   {
      size_t j;
      for (j = 0; j < other.digits.size(); j++)
      {
         if (i + j == result.size())
         {
            result.push_back(0);
         }
         int64 temp = int64(digits[i]) * int64(other.digits[j]) + carry + int64(result[i + j]);
         result[i + j] = temp % BASE;
         carry = temp / BASE;
      }
      while (carry > 0)
      {
         if (i + j == result.size())
         {
            result.push_back(0);
         }
         int64 temp = carry + int64(result[i + j]);
         result[i + j] = temp % BASE;
         carry = temp / BASE;

         j++;
      }
   }

   big_int result_num;
   result_num.is_negative = is_negative ^ other.is_negative;
   result_num.digits = result;

   result_num.normalize();

   return result_num;
}

big_int& big_int::operator*=(const big_int& other)
{
   return *this = *this * other;
}

// service routines

void big_int::normalize()
{
   int new_length = digits.size();
   while (digits[new_length - 1] == 0 && new_length > 1)
   {
      new_length--;
   }
   digits.shrink(new_length);

   if (is_zero()) is_negative = false;
}
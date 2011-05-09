#include "bignum.h"

namespace
{
   int digit_length(int digit)
   {
      if (digit == 0)
      {
         return 1;
      }

      int result = 0;
      while (digit != 0)
      {
         result++;
         digit /= 10;
      }

      return result;
   }
}

// constructors

bignum::bignum(int64 number)
{
   std::vector<int> result;
   while (number != 0)
   {
      result.push_back(number % BASE);
      number /= BASE;
   }

   if (result.size() == 0)
   {
      result.push_back(0);
   }

   digits = result;
}

bignum::bignum(const std::vector<int>& digits_vector)
   : digits(digits_vector)
{
   normalize();
}

void bignum::normalize()
{
   int length = digits.size();
   while (length > 0 && digits[length - 1] == 0)
   {
      length--;
   }

   if (length == 0)
   {
      length = 1;
   }
   digits.resize(length);
}

// input/output routines

std::istream& operator>>(std::istream& in, bignum& big)
{
   std::string buffer;
   if (!(in >> buffer))
   {
      return in;
   }

   int size = buffer.length() / LOG10_BASE + 1;
   int badness = buffer.length() % LOG10_BASE;
   if (badness == 0)
   {
      size--;
   }

   big.digits.resize(size);

   int current = 0, read_digits = 0, index = size - 1;

   for (size_t i = 0; i < buffer.length(); i++)
   {
      if (i % LOG10_BASE == badness && read_digits != 0)
      {
         big.digits[index--] = current;
         current = 0;
         read_digits = 0;
      }
      
      current = current * 10 + buffer[i] - '0';
      read_digits++;
   }

   big.digits[index] = current;

   return in;
}

std::ostream& operator<<(std::ostream& out, const bignum& big)
{
   int zeros;
   for (int i = big.digits.size() - 1; i >= 0; i--)
   {
      zeros = LOG10_BASE - digit_length(big.digits[i]);
      
      if (i != big.digits.size() - 1)
      {
         for (int j = 0; j < zeros; j++)
         {
            out << '0';
         }
      }

      out << big.digits[i];
   }

   return out;
}

// arithmetic routines

bool bignum::is_zero() const
{
   return (digits.size() == 1 && digits[0] == 0);
}

bignum bignum::operator*(int mult) const
{
   if (mult == 0)
   {
      return bignum();
   }

   std::vector<int> result_digits;

   int64 current = 0;
   for (size_t i = 0; i < digits.size(); i++)
   {
      current += int64(digits[i]) * int64(mult);

      result_digits.push_back(current % BASE);
      current /= BASE;
   }
   
   while (current != 0)
   {
      result_digits.push_back(current % BASE);
      current /= BASE;
   }

   return bignum(result_digits);
}

void bignum::subtract(const bignum& subtrahend, size_t shift)
{
   int carry = 0;

   size_t i;
   for (i = 0; i < subtrahend.digits.size(); i++)
   {
      digits[shift + i] -= subtrahend.digits[i] + carry;
      carry = 0;

      if (digits[shift + i] < 0)
      {
         digits[shift + i] += BASE;
         carry = 1;
      }
   }

   while (shift + i < digits.size() && carry > 0)
   {
      digits[shift + i] -= carry;
      carry = 0;

      if (digits[shift + i] < 0)
      {
         digits[shift + i] += BASE;
         carry = 1;
      }
   }

   normalize();
}

int bignum::compare_to(const bignum& other, size_t shift) const
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

std::pair<bignum, bignum> bignum::divide(const bignum& divider)
{
   int shift = digits.size() - 1;
   bignum current = *this;
   std::vector<int> result_digits;

   while (shift >= 0)
   {
      int left = 0, right = BASE;
      while (left + 1 < right)
      {
         int middle = (left + right) / 2;
         if (current.compare_to(divider * middle, shift) >= 0)
         {
            left = middle;
         }
         else
         {
            right = middle;
         }
      }

      if (left != 0 || !result_digits.empty())
      {
         result_digits.push_back(left);
         current.subtract(divider * left, shift);
      }
      shift--;
   }

   if (result_digits.empty())
   {
      result_digits.push_back(0);
   }

   std::reverse(result_digits.begin(), result_digits.end());

   return std::make_pair(bignum(result_digits), current);
}
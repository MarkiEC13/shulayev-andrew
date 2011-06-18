#include "big_int.h"

// containers

big_int::big_int(int64 data)
   : is_negative(data < 0ll)
{
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

// input/output routines

std::istream& operator>>(std::istream& in, big_int& big)
{
   while (isspace(in.peek()))
   {
      in.get();
   }

   if (in.eof())
   {
	   in.setstate(std::ios::failbit);
	   return in;
   }

   big.is_negative = false;
   if (in.peek() == '+' || in.peek() == '-')
   {
      if (in.peek() == '-')
      {
         big.is_negative = true;
      }
      in.get();
   }

   if (in.eof() || !isdigit(in.peek()))
   {
	   in.setstate(std::ios::failbit);
	   return in;
   }

   std::vector<char> buffer;
   while (isdigit(in.peek()))
   {
      if (in.peek() != '0' || !buffer.empty())
      {
         buffer.push_back(char(in.get()));
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

      size_t badness = buffer.size() % LOG10_BASE;
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

   std::streamsize old_width = out.width();
   char old_fill = out.fill();

   for (int i = big.digits.size() - 2; i >= 0; i--)
   {
      out << std::setw(LOG10_BASE) << std::setfill('0') << big.digits[i];
   }

   out.width(old_width);
   out.fill(old_fill);

   return out;
}

// comparison operators

int big_int::digits_compare(const big_int& other, size_t shift = 0) const
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

int big_int::compare_to(const big_int& other) const
{
   if (is_negative && !other.is_negative)
   {
      return -1;
   }
   else if (!is_negative && other.is_negative)
   {
      return 1;
   }
   else if (is_negative)
   {
      return -digits_compare(other);
   }
   else
   {
      return digits_compare(other);
   }
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

big_int abs(const big_int& arg)
{
   big_int result = arg;

   if (!result.is_zero())
   {
      result.is_negative = false;
   }

   return result;
}

big_int& big_int::operator+=(const big_int& other)
{
   if (other.is_zero()) return *this;

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
   if (other.is_zero()) return *this;

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

// we are so close to implement division
// some auxiliary methods first

void big_int::inplace_subtract(const big_int& other, size_t shift = 0)
{
   int carry = 0;

   size_t i;
   for (i = 0; i < other.digits.size(); i++)
   {
      digits[shift + i] -= other.digits[i] + carry;
      carry = 0;

      if (digits[shift + i] < 0)
      {
         digits[shift + i] += BASE;
         carry = 1;
      }
   }

   while (shift + i < digits.size() && carry != 0)
   {
      digits[shift + i] -= carry;
      carry = 0;

      if (digits[shift + i] < 0)
      {
         digits[shift + i] += BASE;
         carry = 1;
      }
   }
}

big_int big_int::operator*(int mult) const
{
   if (mult == 0) return big_int(0);

   big_int result = *this;

   if (mult < 0)
   {
      result.is_negative = !result.is_negative;
      mult = -mult;
   }

   int64 carry = 0;
   for (size_t i = 0; i < digits.size(); i++)
   {
      carry += int64(digits[i]) * int64(mult);

      result.digits[i] = carry % BASE;
      carry /= BASE;
   }

   while (carry > 0)
   {
      result.digits.push_back(carry % BASE);
      carry /= BASE;
   }

   return result;
}

big_int big_int::inplace_remainder(const big_int& other)
{
   int shift = digits.size() - other.digits.size();
   container_t result_digits;
   bool result_is_negative = is_negative ^ other.is_negative;

   while (shift >= 0)
   {
      int left = 0, right = BASE;
      while (left + 1 < right)
      {
         int middle = (left + right) / 2;
         if (this->digits_compare(other * middle, shift) >= 0)
         {
            left = middle;
         }
         else
         {
            right = middle;
         }
      }

      if (left != 0 || result_digits.size() > 0)
      {
         result_digits.push_back(left);
         this->inplace_subtract(other * left, shift);

         normalize();
      }

      shift--;
   }

   if (result_digits.size() == 0)
   {
      result_digits.push_back(0);
      result_is_negative = false;
   }

   result_digits.reverse();

   big_int result;
   result.digits = result_digits;
   result.is_negative = result_is_negative;

   return result;
}

big_int& big_int::operator%=(const big_int& other)
{
   this->inplace_remainder(other);
   return *this;
}

big_int big_int::operator%(const big_int& other) const
{
   big_int result(*this);
   return result %= other;
}

big_int big_int::operator/(const big_int& other) const
{
   big_int temp(*this);
   return temp.inplace_remainder(other);
}

big_int& big_int::operator/=(const big_int& other)
{
   return *this = *this / other;
}

std::pair<big_int, big_int> big_int::divide_with_remainder(const big_int& other) const
{
   big_int second(*this);
   big_int first(second.inplace_remainder(other));

   return std::make_pair(first, second);
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
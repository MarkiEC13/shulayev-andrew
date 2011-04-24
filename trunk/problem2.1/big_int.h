#ifndef BIG_INT_H
#define BIG_INT_H

#include <vector>
#include <iomanip>
#include <fstream>
#include <algorithm>

#include "container_t.h"

typedef long long int64;

const int LOG10_BASE = 9;
const int BASE = 1000000000;

class big_int
{
public:
   big_int();
   big_int(int);
   big_int(int64);
   friend std::istream& operator>>(std::istream&, big_int&);
   friend std::ostream& operator<<(std::ostream&, const big_int&);

   bool operator==(const big_int&) const;
   bool operator<=(const big_int&) const;
   bool operator>=(const big_int&) const;
   bool operator!=(const big_int&) const;
   bool operator<(const big_int&) const;
   bool operator>(const big_int&) const;
   bool is_zero() const;

   big_int& operator=(const big_int&);
   void swap(big_int&);

   big_int operator-() const;
   big_int& operator+=(const big_int&);
   big_int& operator-=(const big_int&);
   big_int& operator*=(const big_int&);
   big_int& operator/=(const big_int&);
   big_int& operator%=(const big_int&);
   big_int operator+(const big_int&) const;
   big_int operator-(const big_int&) const;
   big_int operator*(const big_int&) const;
   big_int operator/(const big_int&) const;
   big_int operator%(const big_int&) const;
   std::pair<big_int, big_int> divide_with_remainder(const big_int&) const;

   friend big_int abs(const big_int&);

private:
   bool is_negative;
   container_t digits;

   void inplace_subtract(const big_int&, size_t);
   big_int operator*(int) const;
   big_int inplace_remainder(const big_int&);

   int compare_to(const big_int&, size_t) const;
   void normalize();
};

#endif
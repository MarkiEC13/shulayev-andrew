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

   big_int& operator=(const big_int&);
   void swap(big_int&);

private:
   bool is_negative;
   container_t digits;

   int compare_to(const big_int&) const;
};

#endif
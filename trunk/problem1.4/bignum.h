#ifndef BIGNUM_H
#define BIGNUM_H

const int LOG10_BASE = 9;
const int BASE = 1000000000;

typedef long long int64;

#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

class bignum
{
public:
   friend std::istream& operator>>(std::istream&, bignum&);
   friend std::ostream& operator<<(std::ostream&, const bignum&);
   std::pair<bignum, bignum> divide(const bignum&);
   bignum operator*(int mult) const;
   bool is_zero() const;
   bignum(int);
   bignum(int64 number = 0);
   bignum(const std::vector<int>&);
   
private:
   int compare_to(const bignum&, size_t);
   void subtract(const bignum&, size_t);
   std::vector<int> digits;
   size_t length;
};

#endif
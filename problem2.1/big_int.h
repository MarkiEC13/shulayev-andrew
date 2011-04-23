#ifndef BIG_INT_H
#define BIG_INT_H

#include <algorithm>

typedef long long int64;

const int LOG10_BASE = 9;
const int BASE = 1000000000;

class big_int
{
public:
   big_int();
   big_int(int);
   big_int(int64);
   friend std::istream& operator>>(std::istream&, bignum&);
   friend std::ostream& operator<<(std::ostream&, const bignum&);

private:
   bool is_negative;
   container_t digits;
}

#endif
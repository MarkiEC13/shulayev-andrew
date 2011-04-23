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
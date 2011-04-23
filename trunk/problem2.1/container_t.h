#ifndef CONTAINER_T_H
#define CONTAINER_T_H

#include <cassert>
#include <algorithm>

class container_t
{
public:
   container_t();
   container_t(const container_t&);
   void push_back(size_t);
   int& operator[](size_t);
   int operator[](size_t) const;
   container_t& operator=(const container_t&);
   void reverse();
   void ensure_capacity(size_t);
   ~container_t();
   size_t size() const;
   void swap(container_t&);
   void shrink(size_t);

private:
   size_t length;
   size_t capacity;
   union
   {
      int digit;
      int* digits;
   };
};

#endif
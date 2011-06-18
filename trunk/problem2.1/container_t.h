#ifndef CONTAINER_T_H
#define CONTAINER_T_H

#include <cassert>
#include <algorithm>

class container_t
{
public:
   container_t();
   container_t(const container_t& other);
   void push_back(size_t item);
   int& operator[](size_t index);
   int operator[](size_t index) const;
   container_t& operator=(const container_t& other);
   void reverse();
   void ensure_capacity(size_t request_size);
   ~container_t();
   size_t size() const;
   void swap(container_t& other);
   void shrink(size_t request_size);

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
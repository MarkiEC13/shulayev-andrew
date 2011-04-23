#ifndef CONTAINER_T_H
#define CONTAINER_T_H

#include <algorithm>

class container_t
{
public:
   container_t();
   container_t(const container_t&);
   void push_back(size_t);
   size_t& operator[](size_t);
   size_t operator[](size_t) const;
   container_t& operator=(const container_t&);
   void reverse();
   void ensure_capacity(size_t);
   ~container_t();
   size_t size() const;
   void swap(container_t&);

private:
   size_t length;
   size_t capacity;
   union
   {
      size_t digit;
      size_t* digits;
   };
};

#endif
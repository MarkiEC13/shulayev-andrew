#include "container_t.h"

// containers

container_t::container_t()
{
   length = 0;
   capacity = 1;
}

container_t::container_t(const container_t& other)
{
   length = other.length;
   capacity = other.capacity;

   if (length == 1)
   {
      digit = other.digit;
   }
   else
   {
      digits = new size_t[capacity];
      for (size_t i = 0; i < capacity; i++)
      {
         digits[i] = other.digits[i];
      }
   }
}

// methods

void container_t::ensure_capacity(size_t size)
{
   if (size <= capacity)
   {
      return;
   }

   size_t new_capacity = capacity;
   while (size > new_capacity)
   {
      new_capacity *= 2;
   }

   size_t* new_digits = new size_t[new_capacity];

   if (capacity == 1)
   {
      new_digits[0] = digit;
   }
   else
   {
      for (size_t i = 0; i < capacity; i++)
      {
         new_digits[i] = digits[i];
      }

      delete[] digits;
   }

   capacity = new_capacity;
   digits = new_digits;
}

void container_t::push_back(size_t item)
{
   if (length == 0)
   {
      digit = item;
   }
   else
   {
      ensure_capacity(length + 1);
      digits[length] = item;
   }

   length++;
}

size_t& container_t::operator[](size_t index)
{
   if (capacity == 1)
   {
      return digit;
   }
   else
   {
      return digits[index];
   }
}

container_t::~container_t()
{
   if (capacity > 1)
   {
      delete[] digits;
   }
}

size_t container_t::size()
{
   return length;
}
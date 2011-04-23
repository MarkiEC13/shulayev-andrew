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

   if (capacity == 1)
   {
      digit = other.digit;
   }
   else
   {
      digits = new int[capacity];
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

   int* new_digits = new int[new_capacity];

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
   if (length == 0 && capacity == 1)
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

int& container_t::operator[](size_t index)
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

int container_t::operator[](size_t index) const
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

container_t& container_t::operator=(const container_t& other)
{
   if (capacity > 1)
   {
      delete[] digits;
   }
   length = other.length;
   capacity = other.capacity;

   if (capacity == 1)
   {
      digit = other.digit;
   }
   else
   {
      digits = new int[capacity];
      for (size_t i = 0; i < capacity; i++)
      {
         digits[i] = other.digits[i];
      }
   }

   return *this;
}

void container_t::swap(container_t& other)
{
   std::swap(length, other.length);
   std::swap(capacity, other.capacity);
   std::swap(digit, digit);
}

void container_t::reverse()
{
   for (size_t i = 0; i < length / 2; i++)
   {
      std::swap(digits[i], digits[length - i - 1]);
   }
}

container_t::~container_t()
{
   if (capacity > 1)
   {
      delete[] digits;
   }
}

size_t container_t::size() const
{
   return length;
}

void container_t::shrink(size_t size)
{
   assert(size <= length);
   length = size;
}
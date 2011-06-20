#include "container_t.h"

// containers

container_t::container_t()
   : length(0), capacity(1)
{ }

container_t::container_t(const container_t& other)
   : length(other.length)
{
   capacity = 1;
   ensure_capacity(length);
   for (size_t i = 0; i < length; i++)
   {
      (*this)[i] = other[i];
   }
}

// methods

void container_t::ensure_capacity(size_t request_size)
{
   if (request_size <= capacity)
   {
      return;
   }

   size_t new_capacity = capacity;
   while (request_size > new_capacity)
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
   ensure_capacity(length + 1);
   (*this)[length++] = item;
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
   length = other.length;
   ensure_capacity(length);

   for (size_t i = 0; i < length; i++)
   {
      (*this)[i] = other[i];
   }

   return *this;
}

void container_t::swap(container_t& other)
{
   std::swap(length, other.length);
   std::swap(capacity, other.capacity);

   if (sizeof(digit) > sizeof(digits))
   {
      std::swap(digit, other.digit);
   }
   else
   {
      std::swap(digits, other.digits);
   }
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

void container_t::shrink(size_t request_size)
{
   assert(request_size <= length);

   length = request_size;
}
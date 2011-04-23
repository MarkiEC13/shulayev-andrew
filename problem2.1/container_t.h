#ifndef CONTAINER_T_H
#define CONTAINER_T_H

class container_t
{
public:
   container_t();
   container_t(const container_t&);
   void push_back(size_t);
   size_t& operator[](size_t);
   ~container_t();
   size_t size();

private:
   size_t length;
   size_t capacity;
   union
   {
      size_t digit;
      size_t* digits;
   };

   void ensure_capacity(size_t);
};

#endif
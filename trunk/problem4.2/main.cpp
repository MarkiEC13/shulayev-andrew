#include <iostream>

struct nil { };

template<int head, typename tail> struct cons
{
   static const int car = head;
   typedef tail cdr;
}; // lisp 4ever

template<typename list, bool second> struct split;

template<> struct split<nil, true>
{
   typedef nil type;
};

template<> struct split<nil, false>
{
   typedef nil type;
};

template<typename list> struct split<list, false>
{
   typedef typename cons<
      list::car,
      typename split<
         typename list::cdr,
         true
      >::type
   > type;
};

template<typename list> struct split<list, true>
{
   typedef typename split<
      typename list::cdr,
      false
   >::type type;
};

template<typename left, typename right, bool first> struct merge_helper;
template<typename left, typename right> struct merge
{
   typedef typename merge_helper<
      left,
      right,
      (left::car) < (right::car)
   >::list list;
};

template<typename left> struct merge<left, nil>
{
   typedef left list;
};

template<typename right> struct merge<nil, right>
{
   typedef right list;
};

template<typename left, typename right, bool first> struct merge_helper;

template<typename left, typename right> struct merge_helper<left, right, true>
{
   typedef typename cons<
      left::car,
      typename merge<
         typename left::cdr,
         right
      >::list
   > list;
};

template<typename left, typename right> struct merge_helper<left, right, false>
{
   typedef typename cons<
      right::car,
      typename merge<
         left,
         typename right::cdr
      >::list
   > list;
};

template<typename list> struct sort
{
   typedef typename merge<
      typename sort<
         typename split<
            list,
            false
         >::type
      >::list,
      typename sort<
         typename split<
            list,
            true
         >::type
      >::list
   >::list list;
};

template<> struct sort<nil>
{
   typedef nil list;
};

template<typename list> void output()
{
   std::cout << list::car << ' ';
   output<list::cdr>();
}

template<> void output<nil>()
{
   std::cout << '\n';
}

typedef cons<9, cons<121, cons<900, cons<1209, cons<1, cons<-1, cons<-128, cons<451, nil>>>>>>>> list;

int main()
{
   output<sort<list>::list>();
   return 0;
}
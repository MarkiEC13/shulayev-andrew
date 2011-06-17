#include <cstdio>

// list structure

struct nil { };

template<int head, typename tail> struct cons
{
   static const int car = head;
   typedef tail cdr;
};

// long numbers construction

template<int num> struct long_num
{
   typedef typename cons<
      num % 10,
      typename long_num<
         num / 10
      >::type
   > type;
};

template<> struct long_num<0>
{
   typedef nil type;
};

// long numbers addition

template<typename num1, typename num2, int carry> struct sum
{
   typedef typename cons<
      (num1::car + num2::car + carry) % 10,
      typename sum<
         typename num1::cdr,
         typename num2::cdr,
         (num1::car + num2::car + carry) / 10
      >::type
   > type;
};

template<typename num1, int carry> struct sum<num1, nil, carry>
{
   typedef typename cons<
      (num1::car + carry) % 10,
      typename sum<
         typename num1::cdr,
         nil,
         (num1::car + carry) / 10
      >::type
   > type;
};

template<typename num2, int carry> struct sum<nil, num2, carry>
{
   typedef typename cons<
      (num2::car + carry) % 10,
      typename sum<
         nil,
         typename num2::cdr,
         (num2::car + carry) / 10
      >::type
   > type;
};

template<int carry> struct sum<nil, nil, carry>
{
   typedef typename long_num<carry>::type type;
};

// long numbers multiplication
// long-short number multiplication

template<typename num1, int num2, int carry> struct mult_short
{
   typedef typename cons<(num1::car * num2 + carry) % 10, typename mult_short<typename num1::cdr, num2, (num1::car * num2 + carry) / 10>::type> type;
};

template<typename num1, int carry> struct mult_short<num1, 0, carry>
{
   typedef nil type;
};

template<int num2, int carry> struct mult_short<nil, num2, carry>
{
   typedef typename long_num<carry>::type type;
};

template<int carry> struct mult_short<nil, 0, carry>
{
   typedef nil type;
};

// long-long number multiplication

// helper function
// *10 multiplication
template<typename num1> struct extend
{
   typedef cons<0, num1> type;
};

template<> struct extend<nil>
{
   typedef nil type;
};

template<typename num1, typename num2> struct mult
{
   typedef typename sum<typename mult_short<num1, num2::car, 0>::type, typename extend<typename mult<num1, typename num2::cdr>::type>::type, 0>::type type;
};

template<typename num1> struct mult<num1, nil>
{
   typedef nil type;
};

template<typename num2> struct mult<nil, num2>
{
   typedef nil type;
};

template<> struct mult<nil, nil>
{
   typedef nil type;
};

// long numbers comparison

// some auxiliary metafunctions first

template<bool condition, int if_clause, int else_clause> struct static_if;

template<int if_clause, int else_clause> struct static_if<true, if_clause, else_clause>
{
   static const int value = if_clause;
};

template<int if_clause, int else_clause> struct static_if<false, if_clause, else_clause>
{
   static const int value = else_clause;
};

template<int num1, int num2, int prefix_result> struct combine_comparison;

template<int num1, int num2> struct combine_comparison<num1, num2, 1>
{
   static const int value = 1;
};

template<int num1, int num2> struct combine_comparison<num1, num2, -1>
{
   static const int value = -1;
};

template<int num> struct combine_comparison<num, num, 0>
{
   static const int value = 0;
};

template<int num1, int num2> struct combine_comparison<num1, num2, 0>
{
   static const int value = static_if<num1 < num2, -1, 1>::value;
};

template<typename num1, typename num2> struct compare
{
   static const int value = combine_comparison<num1::car, num2::car, compare<typename num1::cdr, typename num2::cdr>::value>::value;
};

template<typename num1> struct compare<num1, nil>
{
   static const int value = 1;
};

template<typename num2> struct compare<nil, num2>
{
   static const int value = -1;
};

template<> struct compare<nil, nil>
{
   static const int value = 0;
};

// long numbers subtraction

// some auxiliary metafunctions first

template<bool condition, typename if_clause, typename else_clause> struct static_types_if;

template<typename if_clause, typename else_clause> struct static_types_if<true, if_clause, else_clause>
{
   typedef if_clause type;
};

template<typename if_clause, typename else_clause> struct static_types_if<false, if_clause, else_clause>
{
   typedef else_clause type;
};

template<typename num1, typename num2, int carry> struct subtract_helper
{
   typedef typename cons<num1::car - (num2::car + carry) + static_if<num1::car >= (num2::car + carry), 0, 10>::value, typename subtract_helper<typename num1::cdr, typename num2::cdr, static_if<num1::car >= (num2::car + carry), 0, 1>::value>::type> type;
};

template<typename num1, int carry> struct subtract_helper<num1, nil, carry>
{
   typedef typename cons<num1::car - carry + static_if<num1::car >= carry, 0, 10>::value, typename subtract_helper<typename num1::cdr, nil, static_if<num1::car >= carry, 0, 1>::value>::type> type;
};

template<int carry> struct subtract_helper<nil, nil, carry>
{
   typedef nil type;
};

template<typename first, typename second> struct lazy_subtraction
{
   typedef first num1;
   typedef second num2;
};

template<typename A, typename B> struct static_equal
{
   static const bool value = false;
};

template<typename A> struct static_equal<A, A>
{
   static const bool value = true;
};

template<typename num> struct strip_zeroes
{
   typedef typename static_types_if<
      static_equal<
         typename strip_zeroes<
            typename num::cdr
         >::type,
         nil
      >::value && (num::car == 0),
      nil,
      typename cons<
         num::car,
         typename strip_zeroes<
            typename num::cdr
         >::type
      >
   >::type type;
};

template<> struct strip_zeroes<nil>
{
   typedef nil type;
};

template<typename delayed> struct get_result
{
   typedef typename strip_zeroes<typename subtract_helper<typename delayed::num1, typename delayed::num2, 0>::type>::type type;
};

template<> struct get_result<nil>
{
   typedef nil type;
};

// we consider there are no numbers except N U {0}
template<typename num1, typename num2> struct subtract
{
   typedef typename get_result<typename static_types_if<(compare<num1, num2>::value > 0), typename lazy_subtraction<num1, num2>, nil>::type>::type type;
};

template<typename num> struct subtract<num, num>
{
   typedef nil type;
};

// long numbers division

template<typename num1, typename num2> struct find_digit
{
   static const int value = static_if<
      compare<num1, num2>::value >= 0,
      1 + find_digit<
         typename subtract<num1, num2>::type,
         num2
      >::value,
      0
   >::value;
};

template<typename num2> struct find_digit<nil, num2>
{
   static const int value = 0;
};

template<typename num1, typename num2> struct divide
{
   typedef typename divide<
      typename num1::cdr,
      num2
   >::type prefix;

   typedef typename sum<
      typename long_num<
         find_digit<
            typename subtract<
               num1,
               typename mult<num2, prefix>::type
            >::type,
            num2
         >::value
      >::type,
      typename extend<prefix>::type,
      0
   >::type type;
};

template<typename num2> struct divide<nil, num2>
{
   typedef nil type;
};

// long numbers output

template<typename list> int print()
{
   int a = print<typename list::cdr>();
   putchar('0' + list::car);
   return a + 1;
}

template<> int print<nil>() { return 0; }

template<typename list> void println()
{
   if (print<list>() == 0)
   {
      putchar('0');
   }
   putchar('\n');
}

typedef long_num<123>::type num1;
typedef long_num<345>::type num2;
typedef sum<num1, num2, 0>::type sum1;

typedef long_num<1>::type one;
typedef long_num<99>::type near_hundred;
typedef sum<one, near_hundred, 0>::type sum2;

typedef subtract<sum2, near_hundred>::type new_one;

typedef mult_short<sum1, 3, 0>::type mult1;
typedef mult<num1, num2>::type mult2; // 42435
typedef mult<num2, num1>::type mult3; // 42435

typedef subtract<mult2, sum2>::type subtr1; // 42335
typedef subtract<sum2, mult2>::type subtr2;

typedef divide<num2, num1>::type div1;

int main()
{
   println<mult2>();
   println<new_one>();
   println<subtr1>();
   println<subtr2>();
   println<div1>();
   return 0;
}
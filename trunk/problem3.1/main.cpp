#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>
#include <algorithm>

#include "big_int.h"

namespace calc
{
   namespace qi = boost::spirit::qi;
   namespace ascii = qi::ascii;
   namespace phoenix = boost::phoenix;

   struct get_number_impl
   {
      template<typename Value> struct result
      {
         typedef big_int type;
      };

      template<typename Value> big_int operator()(Value val) const
      {
         return boost::lexical_cast<big_int>(val);
      }
   };
   phoenix::function<get_number_impl> get_number;

   struct binary_pow_impl
   {
      template<typename Base, typename Log> struct result
      {
         typedef Base type;
      };

      template<typename Base, typename Log> Base operator()(Base base, Log log) const
      {
         Base result = Base(1);

         while (log > Log(0))
         {
            if (log % Log(2) == Log(1))
            {
               result = result * base;
            }

            base = base * base;
            log = log / Log(2);
         }

         return result;
      }
   };
   phoenix::function<binary_pow_impl> binary_pow;

   template<typename Iterator> struct calculator : qi::grammar<Iterator, big_int(), ascii::space_type>
   {
      qi::rule<Iterator, big_int(), ascii::space_type> start, expr, term, fact, power, number;
      qi::rule<Iterator, std::string(), ascii::space_type> digits;

      calculator() : calculator::base_type(start, "Calculate expression")
      {
         using qi::_1;
         using qi::_2;
         using qi::_val;

         start  %= expr > qi::eoi;
         expr    = term[_val = _1] >> *(('+' >> term)[_val += _1] | ('-' >> term)[_val -= _1]);
         term    = fact[_val = _1] >> *(('*' >> fact)[_val *= _1] | ('/' >> fact)[_val /= _1]);
         fact    = power[_val = _1] >> -('^' >> fact[_val = binary_pow(_val, _1)]);
         power  %= number;
         number %= digits[_val = get_number(_1)] | ('-' >> number[_val = -_1]);
         digits  = +(ascii::digit);
      }
   };
}

int main()
{
   using boost::spirit::ascii::space;

   typedef std::string::const_iterator iter_t;
   typedef calc::calculator<iter_t> calculator;

   calculator g;
   std::string s;

   while (std::cout << "> ", std::getline(std::cin, s), !s.empty())
   {
      big_int ret;
      iter_t begin = s.begin(), end = s.end();

      try
      {
         phrase_parse(begin, end, g, space, ret);
         std::cout << ret << '\n';
      }
      catch (const std::exception&)
      {
         std::cout << "<ERROR>\n";
      }
   }

   return 0;
}
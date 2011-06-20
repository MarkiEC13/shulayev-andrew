#include "func_factory.h"

struct func_fact : i_function
{
   big_int operator()(big_int arg)
   {
      big_int result(1);
      
      while (arg > 0)
      {
         result *= arg;
         arg -= big_int(1);
      }

      return result;
   }
};

struct func_sqr : i_function
{
   big_int operator()(big_int arg)
   {
      return arg * arg;
   }
};

struct func_digits : i_function
{
   big_int operator()(big_int arg)
   {
      arg = abs(arg);

      int result = 0;
      while (arg > 0)
      {
         result += 1;
         arg /= 10;
      }

      if (result == 0)
      {
         return 1;
      }

      return result;
   }
};

// function fabric methods

function_factory::function_factory()
{
   functions["fact"] = new func_fact();
   functions["sqr"] = new func_sqr();
   functions["digits"] = new func_digits();
}

function_factory::function_factory(const function_factory&) { }

function_factory::~function_factory()
{
   for (std::map<std::string, i_function*>::iterator it = functions.begin();
        it != functions.end(); it++)
   {
      delete it->second;
   }
}

function_factory& function_factory::get_instance()
{
   static function_factory instance;
   return instance;
}

i_function* function_factory::get_function_helper(std::string name)
{
   return functions[name];
}

// access to functions from outside
i_function* get_function(std::string name)
{
   return function_factory::get_instance().get_function_helper(name);
}
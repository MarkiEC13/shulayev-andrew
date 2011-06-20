#ifndef FUNC_FACTORY_H
#define FUNC_FACTORY_H

#include <map>
#include <string>

#include "big_int.h"

struct i_function
{
   virtual big_int operator()(big_int arg) = 0;
};

class function_factory
{
public:
   static function_factory& get_instance();
   i_function* get_function_helper(std::string name);
   ~function_factory();

private:
   function_factory();
   function_factory(const function_factory&);
   std::map<std::string, i_function*> functions;
};

i_function* get_function(std::string name);

#endif
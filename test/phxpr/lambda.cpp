////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <sheol/lightweight_test.hpp>

#include <prana/utree/io.hpp>

#include <phxpr/evaluator.hpp>
#include <phxpr/primitives/lambda.hpp>
#include <phxpr/primitives/placeholder.hpp>

using boost::spirit::utree;
using boost::spirit::scope;

using phxpr::evaluator;
using phxpr::lambda;
using phxpr::placeholder;
using phxpr::function_body;

using phxpr::signature;
using phxpr::displacement;
using phxpr::arity_type;
using phxpr::evaluation_strategy;

struct addition_function {
  utree operator() (scope const& env) const {
    BOOST_ASSERT(env.size() == 2);
    return env[0] + env[1];
  }
};

int main (void) {
  evaluator e;

  // specify the lambda signature 
  signature sig(2, arity_type::fixed, evaluation_strategy::call_by_value);  

  // allocate the lambda body
  boost::shared_ptr<function_body> body
    = boost::make_shared<function_body>(addition_function());

  // create a new lambda expression
  lambda l(body, sig);

  // evaluate the lambda expression, returning a procedure
  utree proc = l.eval(scope());

  // create our arguments
  boost::array<utree, 2> args = { { utree(2), utree(4) } };

  // call the procedure
  utree result = proc.eval(scope(args.begin(), args.end()));

  std::cout << result << std::endl;

  return sheol::report_errors();
}



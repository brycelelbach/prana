/*==============================================================================
    Copyright (c) 2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_6ADC238A_29FE_4E20_ABE6_EC49705D14C3)
#define BSP_6ADC238A_29FE_4E20_ABE6_EC49705D14C3

#include <boost/spirit/home/prana/phxpr/core/unary_function.hpp>
#include <boost/spirit/home/prana/generate/generate_sexpr.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

// temporary intrinsic, superseded by ports
struct display_function: unary_function<display_function> {
  display_function (boost::shared_ptr<actor_list> const& elements):
    base_type(elements) { }

  typedef utree result_type;

  utree eval (utree const& subject) const {
    if (subject.which() == utree_type::string_type ||
        subject.which() == utree_type::string_range_type)
    {
      utf8_string_range_type str = subject.get<utf8_string_range_type>();
      std::cout << std::string(str.begin(), str.end());
    }
 
    else
      generate_sexpr(subject, std::cout);

    return utree(spirit::nil);
  }
};

struct display_composite: unary_composite<display_function> { };

display_composite const display = display_composite();

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_6ADC238A_29FE_4E20_ABE6_EC49705D14C3


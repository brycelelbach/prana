/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_INPUT_SAVE_LINE_POS_HPP)
#define BOOST_SPIRIT_PRANA_INPUT_SAVE_LINE_POS_HPP

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct save_line_pos_functor {
  template<class, class>
  struct result {
    typedef void type;
  };

  template<class Range>
  void operator() (utree& ast, Range const& rng) const {
    int n = get_line(rng.begin());
    BOOST_ASSERT(n <= (std::numeric_limits<short>::max)());
    ast.tag(n);
  }
};

phoenix::function<save_line_pos_functor> const save_line_pos =
  save_line_pos_functor();

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INPUT_SAVE_LINE_POS_HPP


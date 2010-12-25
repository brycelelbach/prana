/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_INPUT_SAVE_LINE_POS_HPP)
#define BOOST_SPIRIT_PRANA_INPUT_SAVE_LINE_POS_HPP

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_container.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>
#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct push_line_pos_functor {
  template<class, class>
  struct result {
    typedef void type;
  };

  template<class Range>
  void operator() (utree& ast, Range const& rng) const {
    int n = get_line(rng.begin());
    BOOST_ASSERT(n <= (std::numeric_limits<short>::max)());
    try { ast.tag(n); } catch (...) { std::cout << ast << std::endl; }
  }
};

template<class Iterator>
struct save_line_pos: qi::grammar<Iterator, void(utree&, char)> {
  qi::rule<Iterator, void(utree&, char)>
    start;
  
  phoenix::function<push_line_pos_functor>
    push_line_pos;

  save_line_pos (void): save_line_pos::base_type(start) {
    using qi::omit;
    using qi::raw;
    using qi::lit;
    using qi::_1;
    using qi::_r1;
    using qi::_r2;

    start = omit[raw[lit(_r2)] [push_line_pos(_r1, _1)]];
  }
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INPUT_SAVE_LINE_POS_HPP


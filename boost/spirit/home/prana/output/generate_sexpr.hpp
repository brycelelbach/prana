/*==============================================================================
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_OUTPUT_GENERATE_SEXPR_HPP)
#define BOOST_SPIRIT_PRANA_OUTPUT_GENERATE_SEXPR_HPP

#include <boost/spirit/include/karma_generate.hpp>
#include <boost/spirit/include/karma_char.hpp>
#include <boost/spirit/include/karma_list.hpp>
#include <boost/spirit/include/support_ostream_iterator.hpp>

#include <boost/spirit/home/prana/output/grammar/sexpr.hpp>
#include <boost/spirit/home/prana/output/generate_sexpr_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Char>
bool generate_sexpr (utree const& in, std::basic_ostream<Char>& out) {
  typedef spirit::ostream_iterator
    iterator_type;

  sexpr_generator<iterator_type> g;

  return karma::generate(iterator_type(out), g, in);
}

template<class Char>
bool generate_sexpr (utree const& in, std::basic_string<Char>& out) {
  typedef std::basic_string<Char>
    string_type;

  typedef std::back_insert_iterator<string_type>
    iterator_type;

  sexpr_generator<iterator_type> g;

  return karma::generate(iterator_type(out), g, in);
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_OUTPUT_GENERATE_SEXPR_HPP


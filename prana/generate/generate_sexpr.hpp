/*==============================================================================
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_EB93B6AB_F9CA_472E_9D47_844CB954C501)
#define PRANA_EB93B6AB_F9CA_472E_9D47_844CB954C501

#include <prana/config.hpp>

#include <iosfwd>

#if !defined(PRANA_NO_UTREE_KARMA)
  #include <boost/spirit/include/karma_generate.hpp>
  #include <boost/spirit/include/support_ostream_iterator.hpp>

  #include <prana/generate/grammar/sexpr.hpp>
#else
  #include <sstream>

  #include <prana/generate/visit/sexpr.hpp>
#endif

#include <prana/parse/parse_tree.hpp>
#include <prana/parse/grammar/sexpr.hpp>

namespace prana {

template<class Char>
bool generate_sexpr (parse_tree<tag::sexpr> const& in,
                     std::basic_ostream<Char>& out)
{ return generate_sexpr(in.ast(), out); }

template<class Char>
bool generate_sexpr (parse_tree<tag::sexpr> const& in,
                     std::basic_string<Char>& out)
{ return generate_sexpr(in.ast(), out); }

template<class Char>
bool generate_sexpr (utree const& in, std::basic_ostream<Char>& out) {
  #if !defined(PRANA_NO_UTREE_KARMA)
    typedef spirit::ostream_iterator iterator_type;
    sexpr_generator<iterator_type> g;
    return karma::generate(iterator_type(out), g, in);
  #else
    sexpr_printer<std::basic_ostream<Char> > print(out);
    print(in);
    return true; 
  #endif
}

template<class Char>
bool generate_sexpr (utree const& in, std::basic_string<Char>& out) {
  #if !defined(PRANA_NO_UTREE_KARMA)
    typedef std::basic_string<Char> string_type;
    typedef std::back_insert_iterator<string_type> iterator_type;
    sexpr_generator<iterator_type> g;
    return karma::generate(iterator_type(out), g, in);
  #else
    std::basic_stringstream<Char> oss;
    sexpr_printer<std::basic_stringstream<Char> > print(oss);
    print(in);
    out = oss.str();
    return true; 
  #endif
}

} // prana

#endif // PRANA_EB93B6AB_F9CA_472E_9D47_844CB954C501


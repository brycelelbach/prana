/*==============================================================================
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_OUTPUT_GENERATE_SEXPR_FWD_HPP)
#define BOOST_SPIRIT_PRANA_OUTPUT_GENERATE_SEXPR_FWD_HPP

#include <iosfwd>
#include <string>

#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Char>
bool generate_sexpr (utree const& in, std::basic_ostream<Char>& out);

template<class Char>
bool generate_sexpr_list (utree const& in, std::basic_ostream<Char>& out);

template<class Char>
bool generate_sexpr (utree const& in, std::basic_string<Char>& out);

template<class Char>
bool generate_sexpr_list (utree const& in, std::basic_string<Char>& out);

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_OUTPUT_GENERATE_SEXPR_FWD_HPP


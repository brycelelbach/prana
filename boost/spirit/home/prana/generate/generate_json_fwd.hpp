/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_OUTPUT_GENERATE_JSON_FWD_HPP)
#define BOOST_SPIRIT_PRANA_OUTPUT_GENERATE_JSON_FWD_HPP

#include <iosfwd>
#include <string>

#include <boost/spirit/include/support_utree.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Char>
bool generate_json (utree const& in, std::basic_ostream<Char>& out);

template<class Char>
bool generate_json (utree const& in, std::basic_string<Char>& out);

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_OUTPUT_GENERATE_JSON_FWD_HPP


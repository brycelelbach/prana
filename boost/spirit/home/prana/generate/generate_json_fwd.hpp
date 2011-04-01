/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_OUTPUT_GENERATE_JSON_FWD_HPP)
#define BSP_OUTPUT_GENERATE_JSON_FWD_HPP

#include <iosfwd>
#include <string>

#include <boost/spirit/home/prana/parse/parse_tree.hpp>
#include <boost/spirit/home/prana/parse/grammar/json.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Char>
bool generate_json (parse_tree<tag::json> const& in,
                    std::basic_ostream<Char>& out);

template<class Char>
bool generate_json (parse_tree<tag::json> const& in,
                    std::basic_string<Char>& out);

} // prana
} // spirit
} // boost

#endif // BSP_OUTPUT_GENERATE_JSON_FWD_HPP


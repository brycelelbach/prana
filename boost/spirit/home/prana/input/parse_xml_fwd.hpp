/*==============================================================================
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/BOOST_LICENSE_1_0.rst)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_INPUT_PARSE_XML_FWD_HPP)
#define BOOST_SPIRIT_PRANA_INPUT_PARSE_XML_FWD_HPP

#include <string>
#include <iosfwd>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Char>
bool parse_xml (std::basic_istream<Char>& in, utree& out,
                std::string const& source = "<string>");

template<class Range>
typename disable_if<is_base_of<std::ios_base, Range>, bool>::type
parse_xml (Range const& in, utree& out, std::string const& source = "<string>");

bool parse_xml (utree const& in, utree& out,
                std::string const& source = "<string>");

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INPUT_PARSE_XML_FWD_HPP


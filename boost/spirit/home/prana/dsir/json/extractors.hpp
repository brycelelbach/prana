/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_DSIR_JSON_EXTRACTORS_HPP)
#define BOOST_SPIRIT_PRANA_DSIR_JSON_EXTRACTORS_HPP

#include <boost/spirit/home/prana/dsir/json/predicates.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct json_ir;

struct key_extractor {
  typedef utf8_symbol_range_type result_type;

  result_type operator() (utree const& ut) const {
    if (!is_member_pair(ut))
      return utf8_symbol_range_type();

    return ut.get<utf8_symbol_range_type>();
  }
 
  result_type operator() (json_ir const& ir) const {
    return (*this)(ir.ast);
  } 
}

template<typename T>
utf8_symbol_range_type extract_key (T const& t) {
  return key_extractor()(t)
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_DSIR_JSON_EXTRACTORS_HPP


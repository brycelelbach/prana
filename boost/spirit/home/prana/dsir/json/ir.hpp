/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_DSIR_JSON_IR_HPP)
#define BOOST_SPIRIT_PRANA_DSIR_JSON_IR_HPP

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>

#include <boost/spirit/home/prana/dsir/json/predicates.hpp>

namespace boost {
namespace spirit {
namespace prana {

class json_ir {
 public:
  friend struct prana::key_extractor;

  typedef multi_index::multi_index_container<
    json_ir,
    multi_index::indexed_by<
      multi_index::random_access<>,
      multi_index::hashed_unique<prana::key_extractor>
    >
  > index_type;

 private:
  utree ast;
};


} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_DSIR_JSON_IR_HPP


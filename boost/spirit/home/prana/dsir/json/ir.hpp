/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_DSIR_JSON_IR_HPP)
#define BOOST_SPIRIT_PRANA_DSIR_JSON_IR_HPP

#include <boost/mpl/at.hpp>

#include <boost/variant.hpp>
#include <boost/variant/get.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/hashed_index.hpp>

#include <boost/spirit/home/support/container.hpp>
#include <boost/spirit/home/prana/adt/dynamic_array.hpp>
#include <boost/spirit/home/prana/dsir/json/predicates.hpp>
#include <boost/spirit/home/prana/dsir/exception.hpp>

namespace boost {
namespace spirit {
namespace prana {

class json_ir;

struct key_extractor {
  typedef utf8_symbol_range_type result_type;

  result_type operator() (utree const& ut) const;
 
  result_type operator() (json_ir const* ir) const;
};

template<typename T>
utf8_symbol_range_type extract_key (T const& t) {
  return key_extractor()(t);
}

struct json_ir_type {
  enum info {
    atom_type = 0,
    object_type = 1,
    array_type = 2
  };
};

class json_ir {
 protected:
  friend struct prana::key_extractor;

  typedef multi_index::multi_index_container<
    json_ir*,
    multi_index::indexed_by<
      multi_index::random_access<>,
      multi_index::hashed_unique<prana::key_extractor>
    >
  > object_type;

  typedef object_type::nth_index<0>::type object_zero_index;
  typedef object_type::nth_index<1>::type object_key_index;

  typedef dynamic_array<json_ir, 4> array_type;

  struct atom_type { }; 

  typedef variant<atom_type, object_type, array_type> index_type;

 private:
  utree ast;
  index_type index;

 protected:
  json_ir_type::info type (void) const;

  object_zero_index& object_by_position (void); 
  object_zero_index const& object_by_position (void) const;
  
  object_key_index& object_by_keys (void); 
  object_key_index const& object_by_keys (void) const;

 public:
  typedef std::size_t size_type;

  json_ir (void);
  json_ir (utree const&);
  json_ir (json_ir const&);
  
  json_ir& operator= (utree const&);
  json_ir& operator= (json_ir const&);

  ~json_ir (void);

  template<class Expr>
  utree const access (Expr const&) const;
};
  
utf8_symbol_range_type key_extractor::operator() (utree const& ut) const {
  if (!is_member_pair(ut))
    return utf8_symbol_range_type();

  return ut.get<utf8_symbol_range_type>();
}
 
utf8_symbol_range_type key_extractor::operator() (json_ir const* ir) const {
  return (ir ? (*this)(ir->ast) : utf8_symbol_range_type());
} 

json_ir_type::info json_ir::type (void) const {
  switch (index.which()) {
    case json_ir_type::atom_type:
      return json_ir_type::atom_type;
    case json_ir_type::object_type:
      return json_ir_type::object_type;
    case json_ir_type::array_type:
      return json_ir_type::array_type;
    default:
      BOOST_THROW_EXCEPTION(expected_variant_type(index.which()));
  }
}

json_ir::object_zero_index& json_ir::object_by_position (void) {
  return boost::get<object_type>(index).get<0>();
}

json_ir::object_zero_index const& json_ir::object_by_position (void) const {
  return boost::get<object_type>(index).get<0>();
}

json_ir::object_key_index& json_ir::object_by_keys (void) {
  return boost::get<object_type>(index).get<1>();
}

json_ir::object_key_index const& json_ir::object_by_keys (void) const {
  return boost::get<object_type>(index).get<1>();
}

json_ir::json_ir (void): ast(), index() { }

json_ir::json_ir (utree const& ut): ast(), index() {
  // TODO: assignment
}

json_ir::json_ir (json_ir const& ir): ast(), index() {
  // TODO: assignment
}

json_ir& json_ir::operator= (utree const& ut) {
  // TODO: assignment
  return *this;
}

json_ir& json_ir::operator= (json_ir const& ir) {
  // TODO: assignment
  return *this;
}

template<class Expr>
utree const json_ir::access (Expr const&) const {
  // TODO: proto stuff
  return utree();
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_DSIR_JSON_IR_HPP


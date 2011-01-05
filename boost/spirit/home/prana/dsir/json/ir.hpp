/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_DSIR_JSON_IR_HPP)
#define BOOST_SPIRIT_PRANA_DSIR_JSON_IR_HPP

#include <functional>

#include <boost/mpl/at.hpp>

#include <boost/variant.hpp>
#include <boost/variant/get.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/hashed_index.hpp>

#include <boost/spirit/home/support/container.hpp>
#include <boost/spirit/home/prana/adt/dynamic_array.hpp>
#include <boost/spirit/home/prana/dsir/json/dsel.hpp>
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

struct key_equal:
  std::binary_function<utf8_symbol_range_type, utf8_symbol_range_type, bool>
{
  typedef bool result_type;
 
  template<typename Iterator>
  result_type operator() (iterator_range<Iterator> const& x,
                          iterator_range<Iterator> const& y) const
  {
    typedef typename iterator_range<Iterator>::const_iterator iterator;

    iterator xit = x.begin(), xend = x.end(),
             yit = y.begin(), yend = y.end();

    while ((xit != xend) && (yit != yend)) {
      if (*xit != *yit)
        return false;
      ++xit; ++yit;
    }

    return (xit == xend) && (yit == yend);
  }
};

struct json_ir_type {
  enum info {
    empty_type = 0,
    atom_type = 1,
    member_pair_type = 2,
    object_type = 3,
    array_type = 4
  };
};

class json_ir { 
 protected:
  friend struct json_access_find;

  typedef multi_index::multi_index_container<
    json_ir*,
    multi_index::indexed_by<
      multi_index::random_access<>,
      multi_index::hashed_unique<
        key_extractor,
        boost::hash<utf8_symbol_range_type>,
        key_equal 
      >
    >
  > object_type;

  typedef object_type::nth_index<0>::type object_zero_index;
  typedef object_type::nth_index<1>::type object_key_index;

  typedef dynamic_array<json_ir, 0> array_type;

  struct empty_type { };
  struct atom_type { }; 

  typedef json_ir* member_pair_type;

  typedef variant<
    empty_type, atom_type, member_pair_type, object_type, array_type
  > index_type;

 private:
  utree ast;
  index_type index;

 protected:
  array_type& array (void);
  array_type const& array (void) const;

  member_pair_type& member_pair (void);
  member_pair_type const& member_pair (void) const;
  
  object_zero_index& object_by_positions (void); 
  object_zero_index const& object_by_positions (void) const;
  
  object_key_index& object_by_keys (void); 
  object_key_index const& object_by_keys (void) const;

  void copy (utree const&);
  void copy (json_ir const&);
 
 public:
  typedef std::size_t size_type;

 protected:
  size_type count (utf8_symbol_range_type const&) const;
  size_type count (char const*) const;
  size_type count (std::string const&) const;
  size_type count (size_type) const;
  
  json_ir const& find (utf8_symbol_range_type const&) const;
  json_ir const& find (char const*) const;
  json_ir const& find (std::string const&) const;
  json_ir const& find (size_type) const;

 public:
  json_ir (void);
  json_ir (utree const&);
  json_ir (json_ir const&);
  
  json_ir& operator= (utree const&);
  json_ir& operator= (json_ir const&);

  ~json_ir (void);
  
  json_ir_type::info type (void) const;

  void clear (void);

  template<typename Arg>
  typename proto::result_of::make_expr<
    proto::tag::subscript, json_access_domain, json_ir const&, Arg const&
  >::type const operator[] (Arg const&) const;

  utree const& get (void) const;

  bool operator== (utree const&) const;
  bool operator== (json_ir const&) const;

  bool operator!= (json_ir const&) const;
  bool operator!= (utree const&) const;
};

// expression wrapper
template<class Expr>
struct json_access_expr {
  BOOST_PROTO_BASIC_EXTENDS(Expr, json_access_expr<Expr>, json_access_domain)

  BOOST_PROTO_EXTENDS_SUBSCRIPT()
  
  operator utree (void) const {
    BOOST_MPL_ASSERT((proto::matches<Expr, json_access_grammar>));
    return json_access_grammar()(*this).get();
  }

  operator json_ir (void) const {
    BOOST_MPL_ASSERT((proto::matches<Expr, json_access_grammar>));
    return json_access_grammar()(*this);
  }

  utree const& get (void) const {
    BOOST_MPL_ASSERT((proto::matches<Expr, json_access_grammar>));
    return json_access_grammar()(*this).get();
  }
};

template<class Expr>
inline std::ostream&
operator<< (std::ostream& out, json_access_expr<Expr> const& e) {
  out << e.get();
  return out;
}
 
utf8_symbol_range_type key_extractor::operator() (utree const& ut) const {
  if (!is_member_pair(ut))
    return utf8_symbol_range_type();

  return ut.front().get<utf8_symbol_range_type>();
}
 
utf8_symbol_range_type key_extractor::operator() (json_ir const* ir) const {
  return (ir ? (*this)(ir->get()) : utf8_symbol_range_type());
} 

json_ir::array_type& json_ir::array (void) {
  return boost::get<array_type>(index);
}

json_ir::array_type const& json_ir::array (void) const {
  return boost::get<array_type>(index);
}

json_ir::member_pair_type& json_ir::member_pair (void) {
  return boost::get<member_pair_type>(index);
}

json_ir::member_pair_type const& json_ir::member_pair (void) const {
  return boost::get<member_pair_type>(index);
}

json_ir::object_zero_index& json_ir::object_by_positions (void) {
  return boost::get<object_type>(index).get<0>();
}

json_ir::object_zero_index const& json_ir::object_by_positions (void) const {
  return boost::get<object_type>(index).get<0>();
}

json_ir::object_key_index& json_ir::object_by_keys (void) {
  return boost::get<object_type>(index).get<1>();
}

json_ir::object_key_index const& json_ir::object_by_keys (void) const {
  return boost::get<object_type>(index).get<1>();
}

void json_ir::copy (utree const& ut) {
  ast = ut;

  if (is_list(ut)) {
    if (is_object(ut)) {
      index = object_type();

      BOOST_FOREACH(utree const& mp, ut) {
        object_by_positions().push_back(new json_ir(mp));
      } 
    }

    else if (is_member_pair(ut)) 
      index = new json_ir(ut.back());

    else /* is_array(ut) */ { 
      index = array_type();
      array_type& a = array();
      a.reserve(ut.size());
      BOOST_FOREACH(utree const& e, ut) {
        a.push_back(e);
      }
    }
  }

  else /* !is_list(ut) */
    index = atom_type();
}

void json_ir::copy (json_ir const& ir) {
  ast = ir.ast;

  if (ir.type() == json_ir_type::object_type) {
    index = object_type();

    object_zero_index::const_iterator it = ir.object_by_positions().begin(),
                                      end = ir.object_by_positions().end();

    for (; it != end; ++it) {
      if (!(*it))
        BOOST_THROW_EXCEPTION(expected_valid_pointer());
      object_by_positions().push_back(new json_ir(**it));
    }
  }

  else if (ir.type() == json_ir_type::member_pair_type)
    index = new json_ir(*ir.member_pair()); 

  else 
    index = ir.index;
}

json_ir::size_type json_ir::count (utf8_symbol_range_type const& key) const {
  if (type() != json_ir_type::object_type)
    return 0;

  return object_by_keys().count(key);
}
  
json_ir::size_type json_ir::count (char const* first) const {
  if (type() != json_ir_type::object_type)
    return 0;

  char const* last = first;
  while (*last)
    last++;

  return object_by_keys().count(utf8_symbol_range_type(first, last));
}

json_ir::size_type json_ir::count (std::string const& str) const {
  if (type() != json_ir_type::object_type)
    return 0;

  char const* first = str.data();
  char const* last = &first[str.length()];

  return object_by_keys().count(utf8_symbol_range_type(first, last));
}

json_ir::size_type json_ir::count (size_type s) const {
  if (type() == json_ir_type::object_type)
    return s < object_by_positions().size(); 

  else if (type() == json_ir_type::array_type)
    return s < array().size(); 

  else 
    return 0;
}

json_ir const& json_ir::find (utf8_symbol_range_type const& key) const {
  return *(**object_by_keys().find(key)).member_pair();
}

json_ir const& json_ir::find (char const* first) const {
  char const* last = first;
  while (*last)
    last++;

  utf8_symbol_range_type key(first, last);
  return *(**object_by_keys().find(key)).member_pair();
}

json_ir const& json_ir::find (std::string const& str) const {
  char const* first = str.data();
  char const* last = &first[str.length()];

  utf8_symbol_range_type key(first, last);
  return *(**object_by_keys().find(key)).member_pair();
}

json_ir const& json_ir::find (size_type s) const {
  if (type() == json_ir_type::object_type)
    return *(*object_by_positions()[s]).member_pair(); 

  return array()[s]; 
}

json_ir::json_ir (void): ast(), index(empty_type()) { }

json_ir::json_ir (utree const& ut): ast(), index(empty_type()) {
  copy(ut);
}

json_ir::json_ir (json_ir const& ir): ast(), index(empty_type()) {
  copy(ir); 
}

json_ir& json_ir::operator= (utree const& ut) {
  if (*this != ut) {
    clear();
    copy(ut);
  }
  return *this;
}

json_ir& json_ir::operator= (json_ir const& ir) {
  if (*this != ir) {
    clear();
    copy(ir);
  }
  return *this;
}

json_ir::~json_ir (void) {
  clear();
}

json_ir_type::info json_ir::type (void) const {
  switch (index.which()) {
    case json_ir_type::empty_type:
      return json_ir_type::empty_type;
    case json_ir_type::atom_type:
      return json_ir_type::atom_type;
    case json_ir_type::member_pair_type:
      return json_ir_type::member_pair_type;
    case json_ir_type::object_type:
      return json_ir_type::object_type;
    case json_ir_type::array_type:
      return json_ir_type::array_type;
    default:
      BOOST_THROW_EXCEPTION(expected_variant_type(index.which()));
  }
}

void json_ir::clear (void) {
  if (type() == json_ir_type::object_type) {
    BOOST_FOREACH(json_ir* mp, object_by_positions()) {
      if (mp) delete mp;
      mp = 0;
    }
  }

  else if (type() == json_ir_type::member_pair_type) {
    json_ir* p = member_pair();
    if (p) delete p;
    p = 0;
  }

  index = empty_type();
}

template<typename Arg>
typename proto::result_of::make_expr<
  proto::tag::subscript, json_access_domain, json_ir const&, Arg const&
>::type const json_ir::operator[] (Arg const& arg) const {
  return proto::make_expr<proto::tag::subscript, json_access_domain>(
    boost::ref(*this), boost::ref(arg)
  ); 
}

utree const& json_ir::get (void) const {
  return ast;
}

bool json_ir::operator== (utree const& rhs) const {
  return ast == rhs;
}

bool json_ir::operator!= (utree const& rhs) const {
  return ast != rhs; 
}

bool json_ir::operator== (json_ir const& rhs) const {
  return ast == rhs.ast;
}

bool json_ir::operator!= (json_ir const& rhs) const {
  return ast != rhs.ast; 
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_DSIR_JSON_IR_HPP


/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_SERIALIZE_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_SERIALIZE_HPP

#include <boost/spirit/home/prana/trampoline.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/sexpr/traits.hpp>
#include <boost/spirit/home/prana/sexpr/core_fwd.hpp>
#include <boost/spirit/home/prana/sexpr/cast.hpp>
#include <boost/spirit/home/prana/sexpr/routine/serialize_fwd.hpp>

#include <boost/spirit/include/karma.hpp> 

namespace boost {
namespace spirit {
namespace prana {

namespace routine {

template<class TagX, class Dummy>
struct serialize {
  struct routine; 

  typedef std::basic_string<char> result_type;
};

//[routine_serialize_boolean_specialization
template<>
struct serialize<sexpr::boolean, prana::unused_type> {
  struct routine; 

  typedef std::basic_string<char> result_type;
  
  result_type operator() (sexpr const& s) const {
    return (*this)(*cast<sexpr::boolean>(s));
  }
  
  result_type operator() (sexpr::boolean::data_type const& b) const {
    using karma::bool_;

    std::basic_string<char> str;
    std::back_insert_iterator<std::basic_string<char> > sink(str); 

    karma::generate(sink, bool_, b);

    return str; 
  }
};
//]

//[routine_serialize_integer_specialization
template<>
struct serialize<sexpr::integer, prana::unused_type> {
  struct routine; 

  typedef std::basic_string<char> result_type;
  
  result_type operator() (sexpr const& s) const {
    return (*this)(*cast<sexpr::integer>(s));
  }
  
  result_type operator() (sexpr::integer::data_type const& i) const {
    std::basic_string<char> str;
    std::back_insert_iterator<std::basic_string<char> > sink(str); 

    karma::int_generator<sexpr::integer::data_type> int_;

    karma::generate(sink, int_, i);

    return str; 
  }
};
//]

//[routine_serialize_floating_specialization
template<>
struct serialize<sexpr::floating, prana::unused_type> {
  struct routine; 

  typedef std::basic_string<char> result_type;
  
  result_type operator() (sexpr const& s) const {
    return (*this)(*cast<sexpr::floating>(s));
  }
  
  result_type operator() (sexpr::floating::data_type const& f) const {
    std::basic_string<char> str;
    std::back_insert_iterator<std::basic_string<char> > sink(str); 

    karma::real_generator<sexpr::floating::data_type> real_;

    karma::generate(sink, real_, f);

    return str;
  }
};
//]

//[routine_serialize_nil_specialization
template<>
struct serialize<sexpr::nil, prana::unused_type> {
  struct routine; 

  typedef std::basic_string<char> result_type;
  
  result_type operator() (sexpr const&) const {
    return "#nil"; 
  }
  
  result_type operator() (sexpr::nil::data_type) const {
    return "#nil";
  }
};
//]

//[routine_serialize_ascii_specialization
template<>
struct serialize<sexpr::ascii, prana::unused_type> {
  struct routine; 

  typedef std::basic_string<char> result_type;
  
  result_type operator() (sexpr const& s) const {
    return (*this)(*cast<sexpr::ascii>(s));
  }
  
  result_type operator() (sexpr::ascii::data_type const& t) const {
    return (*this)(value_at<2>(t), value_at<0>(t));
  }
 
  result_type operator() (char const* c, uinthalf_t s) const {
    using karma::lit;
    using karma::hex;
    using ascii::print;

    std::basic_string<char> str, input(c, c + s);
    std::back_insert_iterator<std::basic_string<char> > sink(str); 

    karma::symbols<char, char const*> esc_char; 

    esc_char.add
      ('\a', "\\a")
      ('\b', "\\b")
      ('\f', "\\f")
      ('\n', "\\n")
      ('\r', "\\r")
      ('\t', "\\t")
      ('\v', "\\v")
      ('\\', "\\\\")
      ('\'', "\\\'")
      ('"', "\\\"")
    ;

    char const* p = c;

    karma::generate(
      sink,
      // begin grammar
      lit("#a\"") << *(esc_char | print | "\\x" << hex) << "\"",
      // end grammar
      input
    );

    return str; 
  }
};
//]

//[routine_serialize_symbol_specialization
template<>
struct serialize<sexpr::symbol, prana::unused_type> {
  struct routine; 

  typedef std::basic_string<char> result_type;
  
  result_type operator() (sexpr const& s) const {
    return (*this)(*cast<sexpr::symbol>(s));
  }
  
  result_type operator() (sexpr::symbol::data_type const& t) const {
    return (*this)(value_at<2>(t), value_at<0>(t));
  }
 
  result_type operator() (char const* c, uinthalf_t s) const {
    return std::basic_string<char>(c, c + s); 
  }
};
//]

//[routine_serialize_vector_specialization
template<>
struct serialize<sexpr::vector, prana::unused_type> {
  struct routine; 

  typedef std::basic_string<char> result_type;
  
  result_type operator() (sexpr const& s) const {
    return (*this)(*cast<sexpr::vector>(s));
  }
  
  result_type operator() (sexpr::vector::data_type const& t) const {
    return (*this)(value_at<2>(t), value_at<0>(t));
  }
 
  result_type operator() (sexpr const* v, uinthalf_t s) const {
    if (!v || !s)
      return "[]";

    std::basic_string<char> str = "[";

    for (uinthalf_t i = 0; i != s; ++s) {
      if (i) str += " ";
      str += prana::serialize(v[i]);
    }
    
    str += "]";

    return str; 
  }
};
//]

//[routine_serialize_cons_specialization
template<class TagX>
struct serialize<
  TagX, typename enable_if<
    is_cons_type<TagX>,
    prana::unused_type
  >::type
> {
  struct routine; 

  typedef std::basic_string<char> result_type;
 
  template<class X> 
  result_type operator() (X const& x) const {
    return (*this)(*cast<TagX>(x));
  }
  
  result_type operator() (typename TagX::data_type const& t) const {
    return (*this)(value_at<0>(t), value_at<1>(t));
  }
  
  template<class X>
  result_type operator() (X const* car, X const* cdr) const {
    if (!car && !cdr)
      return "()";

    std::basic_string<char> str = "(";

    if (car)
      str += prana::serialize(*car);
    else
      str += "#nil";

    str += " . ";

    if (cdr)
      str += prana::serialize(*cdr);
    else
      str += "#nil";

    str += ")";

    return str;
  }
};
//]

} /*<- routine ->*/

//[serialize_definition
template<class X>
inline std::basic_string<char> serialize (X const& x) {
  return dispatch<typename X::registry, routine::serialize, X const>(x);
}
//]

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_SERIALIZE_HPP ->*/


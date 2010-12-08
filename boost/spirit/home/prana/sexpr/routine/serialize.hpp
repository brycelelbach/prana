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
    if (*cast<sexpr::boolean>(s))
      return "#true";
    else
      return "#false";
  }
};
//]

//[routine_serialize_integer_specialization
template<>
struct serialize<sexpr::integer, prana::unused_type> {
  struct routine; 

  typedef std::basic_string<char> result_type;
  
  result_type operator() (sexpr const& s) const {
    std::basic_string<char> str;
    std::back_insert_iterator<std::basic_string<char> > sink(str); 

    karma::int_generator<sexpr::integer::data_type> int_;

    karma::generate(sink,
      // begin grammar
      int_,
      // end grammar
      *cast<sexpr::integer>(s));

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
    std::basic_string<char> str;
    std::back_insert_iterator<std::basic_string<char> > sink(str); 

    karma::real_generator<sexpr::floating::data_type> real_;

    karma::generate(sink,
      // begin grammar
      real_,
      // end grammar
      *cast<sexpr::floating>(s));

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
};
//]

//[routine_serialize_ascii_specialization
template<>
struct serialize<sexpr::ascii, prana::unused_type> {
  struct routine; 

  typedef std::basic_string<char> result_type;
  
  result_type operator() (sexpr const& s) const {
    using karma::lit;
    using karma::hex;
    using ascii::print;

    char const* cstr = value_at<2>(*cast<sexpr::ascii>(s));
    uinthalf_t size = value_at<0>(*cast<sexpr::ascii>(s)); 

    std::basic_string<char> str, input(cstr, cstr + size);
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

    karma::generate(sink,
      // begin grammar
      lit("\"") << *(esc_char | print | "\\x" << hex) << "\"",
      // end grammar
      input);

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
    char const* cstr = value_at<2>(*cast<sexpr::symbol>(s));
    uinthalf_t size = value_at<0>(*cast<sexpr::symbol>(s)); 
    return std::basic_string<char>(cstr, cstr + size); 
  }
};
//]

//[routine_serialize_vector_specialization
template<>
struct serialize<sexpr::vector, prana::unused_type> {
  struct routine; 

  typedef std::basic_string<char> result_type;
  
  result_type operator() (sexpr const& s) const {
    sexpr const* vec = value_at<2>(*cast<sexpr::vector>(s));
    uinthalf_t size = value_at<0>(*cast<sexpr::vector>(s)); 
    
    if (!vec || !size)
      return "[]";

    std::basic_string<char> str = "[";

    for (uinthalf_t i = 0; i != size; ++size) {
      if (i)
        str += " ";

      str += prana::serialize(vec[i]);
    }
    
    str += "]";

    return str; 
  }
};
//]

//[routine_serialize_cons_specialization
template<class Tag>
struct serialize<
  Tag, typename enable_if<
    is_cons_type<Tag>,
    prana::unused_type
  >::type
> {
  struct routine; 

  typedef std::basic_string<char> result_type;
 
  result_type operator() (sexpr const& s) const {
    sexpr const* list = value_at<0>(*cast<Tag>(s));
    
    if (!list)
      return "()";

    return prana::serialize(*list);
  }
};
//]

//[routine_serialize_pair_specialization
template<>
struct serialize<sexpr::pair, prana::unused_type> {
  struct routine; 

  typedef std::basic_string<char> result_type;
 
  result_type operator() (sexpr const& s) const {
    sexpr const* car = value_at<0>(*cast<sexpr::pair>(s));
    sexpr const* cdr = value_at<1>(*cast<sexpr::pair>(s));
    
    if (!car && !cdr)
      return "()";

    std::basic_string<char> str = "(";

    if (car)
      str += prana::serialize(*car);
    else
      str += "#nil";

    str += " ";

    if (cdr)
      str += prana::serialize(*cdr);
    else
      str += "()";

    str += ")";

    return str;
  }
};
//]

} /*<- routine ->*/

//[serialize_definition
inline std::basic_string<char> serialize (sexpr const& s) {
  return dispatch<sexpr::registry, routine::serialize, sexpr const>(s);
}
//]

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_SERIALIZE_HPP ->*/


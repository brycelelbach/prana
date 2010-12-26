/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/BOOST_LICENSE_1_0.rst)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_SERIALIZE_HPP)
#define BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_SERIALIZE_HPP

#include <boost/spirit/home/prana/schematic/routine/serialize_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {

namespace routine {

template<class TagX, class Dummy>
struct serialize {
  struct routine; 

  typedef std::basic_string<char> result_type;

  #if 0
  template<class X>
  result_type operator() (X const&) {
    return result_type();
  }
  #endif
};

//[routine_serialize_boolean_specialization
template<>
struct serialize<schematic::boolean> {
  struct routine; 

  typedef std::basic_string<char> result_type;
  
  result_type operator() (schematic const& s) const {
    if (*cast<schematic::boolean>(s))
      return "#t";
    else
      return "#f";
  }
};
//]

//[routine_serialize_integer_specialization
template<>
struct serialize<schematic::integer> {
  struct routine; 

  typedef std::basic_string<char> result_type;
  
  result_type operator() (schematic const& s) const {
    std::basic_string<char> str;
    std::back_insert_iterator<std::basic_string<char> > sink(str); 

    karma::int_generator<schematic::integer::data_type> int_;

    karma::generate(sink,
      // begin grammar
      int_,
      // end grammar
      *cast<schematic::integer>(s));

    return str; 
  }
};
//]

//[routine_serialize_floating_specialization
template<>
struct serialize<schematic::floating> {
  struct routine; 

  typedef std::basic_string<char> result_type;
  
  result_type operator() (schematic const& s) const {
    std::basic_string<char> str;
    std::back_insert_iterator<std::basic_string<char> > sink(str); 

    karma::real_generator<schematic::floating::data_type> real_;

    karma::generate(sink,
      // begin grammar
      real_,
      // end grammar
      *cast<schematic::floating>(s));

    return str;
  }
};
//]

//[routine_serialize_sentinel_specialization
template<>
struct serialize<schematic::sentinel> {
  struct routine; 

  typedef std::basic_string<char> result_type;
  
  result_type operator() (schematic const&) const {
    return "nil"; 
  }
};
//]

//[routine_serialize_ascii_specialization
template<>
struct serialize<schematic::ascii> {
  struct routine; 

  typedef std::basic_string<char> result_type;
  
  result_type operator() (schematic const& s) const {
    using karma::lit;
    using karma::hex;
    using ascii::print;

    char const* cstr = value_at<2>(*cast<schematic::ascii>(s));
    uinthalf_t size = value_at<0>(*cast<schematic::ascii>(s)); 

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
struct serialize<schematic::symbol> {
  struct routine; 

  typedef std::basic_string<char> result_type;
  
  result_type operator() (schematic const& s) const {
    char const* cstr = value_at<2>(*cast<schematic::symbol>(s));
    uinthalf_t size = value_at<0>(*cast<schematic::symbol>(s)); 
    return std::basic_string<char>(cstr, cstr + size); 
  }
};
//]

//[routine_serialize_vector_specialization
template<>
struct serialize<schematic::vector> {
  struct routine; 

  typedef std::basic_string<char> result_type;
  
  result_type operator() (schematic const& s) const {
    schematic const* vec = value_at<2>(*cast<schematic::vector>(s));
    uinthalf_t size = value_at<0>(*cast<schematic::vector>(s)); 
    
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
    is_cons_type<Tag>
  >::type
> {
  struct routine; 

  typedef std::basic_string<char> result_type;
 
  result_type operator() (schematic const& s) const {
    schematic const* list = value_at<0>(*cast<Tag>(s));
    
    if (!list)
      return "()";

    return prana::serialize(*list);
  }
};
//]

//[routine_serialize_pair_specialization
template<>
struct serialize<schematic::pair> {
  struct routine; 

  typedef std::basic_string<char> result_type;
 
  result_type operator() (schematic const& s) const {
    schematic const* car = value_at<0>(*cast<schematic::pair>(s));
    schematic const* cdr = value_at<1>(*cast<schematic::pair>(s));
    
    if (!car && !cdr)
      return "()";

    std::basic_string<char> str = "(";

    if (car)
      str += prana::serialize(*car);
    else
      str += "nil";

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

} // routine

//[serialize_definition
inline std::basic_string<char> serialize (schematic const& s) {
  return dispatch<schematic::registry, routine::serialize, schematic const>(s);
}
//]

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_SERIALIZE_HPP


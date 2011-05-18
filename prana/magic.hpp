/*==============================================================================
    Copyright (c) 2010 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_C9FC9821_9EA1_4B2B_B51B_4BC88AD3E3C6)
#define PRANA_C9FC9821_9EA1_4B2B_B51B_4BC88AD3E3C6

#include <prana/config.hpp>

#include <boost/foreach.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/has_xxx.hpp>

#include <boost/preprocessor/cat.hpp>

#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at_c.hpp>

#include <boost/spirit/home/qi/parse.hpp>
#include <boost/spirit/home/support/unused.hpp>

#include <sheol/adt/dynamic_array.hpp>

#include <prana/include/utree.hpp>
#include <prana/diagnostics/source_location.hpp>
#include <prana/magic_fwd.hpp>

namespace prana {
namespace magic {

#define PRANA_TRAIT(trait)                                                  \
  BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(BOOST_PP_CAT(is_, trait), trait, false) \
  /***/

PRANA_TRAIT(trait)

template<class Tag, class Enable/* = void*/>
struct annotations_type {
  typedef sheol::adt::dynamic_array<fusion::vector1<source_location> > type;
}; 

template<class Tag, class Enable/* = void*/>
struct source_type {
  typedef std::string type;
};

template<class Tag, class Enable/* = void*/>
struct default_source {
  typedef typename source_type<Tag>::type value_type;

  static value_type const value;
};

template<class Tag, class Enable>
typename default_source<Tag, Enable>::value_type const
default_source<Tag, Enable>::value = "<string>";

template<class Tag, class Enable/* = void*/>
struct has_source_locations: mpl::false_ { };

template<class Tag, class Enable/* = void*/>
struct get_string_from_source {
  typedef std::string type;

  static type call (typename source_type<Tag>::type const& s) {
    return s;
  }  
};

template<class Tag>
std::string stringify_source (typename source_type<Tag>::type const& s) {
  return get_string_from_source<Tag>::call(s);
}

///////////////////////////////////////////////////////////////////////////////
template<class T, class Enable/* = void*/>
struct get_string_from_type {
  typedef std::string type;

  static type call (T& s) {
    return std::string(s);
  }
};

template<>
struct get_string_from_type<std::string> {
  typedef std::string& type;

  static type call (std::string& s) {
    return s;
  }
};

template<>
struct get_string_from_type<std::string const> {
  typedef std::string const& type;

  static type call (std::string const& s) {
    return s;
  }
};

template<>
struct get_string_from_type<char const*> {
  typedef std::string type;

  static type call (char const* s) {
    return std::string(s);
  }
};

template<>
struct get_string_from_type<char*> {
  typedef std::string type;

  static type call (char* s) {
    return std::string(s);
  }
};

template<class T>
typename get_string_from_type<T>::type stringify (T& s) {
  return get_string_from_type<T>::call(s); 
}

///////////////////////////////////////////////////////////////////////////////
template<class Tag, class Enable/* = void*/>
struct extract_source_location_from_node {
  typedef source_location type; 

  static type call (utree const& ut, parse_tree<Tag> const& pt) {
    using fusion::at_c;
    switch (ut.which()) {
      case utree_type::reference_type:
        return call(ut.deref(), pt);
      case utree_type::range_type:
      case utree_type::list_type:
        return at_c<0>(pt.annotations()[ut.tag()]);
      default:
        return make_source_location(-1, -1);
    }
  }
};

template<class Tag>
typename extract_source_location_from_node<Tag>::type
extract_source_location (utree const& ut, parse_tree<Tag> const& pt) {
  return extract_source_location_from_node<Tag>::call(ut, pt);
}

template<class Tag, class Enable/* = void*/>
struct has_list_subtypes: mpl::false_ { };

template<class Tag, class Enable/* = void*/>
struct list_subtypes: mpl::set<> { };

template<class Tag, class Enable/* = void*/>
struct extract_list_subtype_from_node {
  typedef long type; 

  static type call (utree const&, parse_tree<Tag> const&) {
    return -1;
  }
};

template<class Tag>
typename extract_list_subtype_from_node<Tag>::type
extract_list_subtype (utree const& ut, parse_tree<Tag> const& pt) {
  return extract_list_subtype_from_node<Tag>::call(ut, pt);
}

template<class Tag, class Iterator, class Enable/* = void*/>
struct parser_invoker {
  typedef typename parser_type<Tag, Iterator>::type parser_type;

  static bool call (Iterator& first, Iterator last, parser_type& p,
                    utree& ast)
  {
    typedef typename has_whitespace<Tag, Iterator>::type predicate;
    return call(first, last, p, ast, predicate());
  }

  static bool call (Iterator& first, Iterator last, parser_type& p,
                    utree& ast, mpl::false_)
  { return qi::parse(first, last, p, ast); }
  
  static bool call (Iterator& first, Iterator last, parser_type& p,
                    utree& ast, mpl::true_)
  { 
    typedef typename whitespace_type<Tag, Iterator>::type ws_type;
    ws_type ws;
    return qi::phrase_parse(first, last, p, ws, ast);
  }
};

template<class Tag, class Iterator>
bool invoke (Iterator& first, Iterator last,
             typename parser_type<Tag, Iterator>::type& p, utree& ast)
{ return parser_invoker<Tag, Iterator>::call(first, last, p, ast); }

template<class Tag, class OtherTag, class Enable/* = void*/>
struct annotations_builder {
  typedef void type;

  static type call (parse_tree<Tag>& to, parse_tree<OtherTag> const& from) {
    typedef typename annotations_type<Tag>::type
      to_container;

    typedef typename annotations_type<OtherTag>::type
      from_container;
    
    typedef typename annotations_type<Tag>::type::value_type
      to_value_type;
    
    typedef typename annotations_type<OtherTag>::type::value_type
      from_value_type;

    using fusion::at_c;

    to_container& to_c = to.annotations();
    from_container const& from_c = from.annotations();

    to_c.clear();

    BOOST_FOREACH(from_value_type const& e, from_c) {
      to_value_type new_;
      at_c<0>(new_) = at_c<0>(e);
      to_c.push_back(new_);
    }
  }
};

template<class Tag, class OtherTag>
void build_annotations (parse_tree<Tag>& to, parse_tree<OtherTag> const& from) {
  annotations_builder<Tag, OtherTag>::call(to, from);
}

template<class Tag, class Enable/* = void*/>
struct utf_version: mpl::integral_c<unsigned char, 8> { };

template<class Tag, class Iterator, class Enable/* = void*/>
struct has_whitespace: mpl::true_ { };

template<class Tag, class Iterator, class Enable/* = void*/>
struct whitespace_type {
  typedef spirit::standard::space_type type;
}; 

template<class Tag, class Iterator, class Enable/* = void*/>
struct error_handler_type {
  typedef prana::error_handler<Tag, Iterator> type;
}; 

} // magic
} // prana

#endif // PRANA_C9FC9821_9EA1_4B2B_B51B_4BC88AD3E3C6


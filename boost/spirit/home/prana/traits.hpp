/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_TRAITS_HPP)
#define BOOST_SPIRIT_PRANA_TRAITS_HPP

#include <boost/foreach.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/set.hpp>

#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at_c.hpp>

#include <boost/spirit/home/qi/parse.hpp>
#include <boost/spirit/home/support/unused.hpp>
#include <boost/spirit/include/support_utree.hpp>

#include <boost/spirit/home/prana/adt/source_location.hpp>
#include <boost/spirit/home/prana/adt/dynamic_array.hpp>
#include <boost/spirit/home/prana/traits_fwd.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace traits {

template<class Tag, class Enable/* = void*/>
struct annotations_type {
  typedef dynamic_array<fusion::vector1<source_location> > type;
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

template<class Tag, class Enable/* = void*/>
struct extract_source_location_from_node {
  typedef source_location type; 

  static type call (utree const& ut, parse_tree<Tag> const& pt) {
    using fusion::at_c;
    switch (ut.which()) {
      case utree_type::reference_type:
        return call(ut.deref());
      case utree_type::range_type:
      case utree_type::list_type:
        return at_c<0>(pt.annotations()[ut.tag()]);
      default:
        return make_source_location(-1, -1);
    }
  }
};

template<class Tag>
typename prana::result_of::extract_source_location<Tag>::type
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
typename prana::result_of::extract_list_subtype<Tag>::type
extract_list_subtype (utree const& ut, parse_tree<Tag> const& pt) {
  return extract_list_subtype_from_node<Tag>::call(ut, pt);
}

template<class Tag, class Iterator, class Enable/* = void*/>
struct parser_invoker {
  typedef typename parser_type<Tag, Iterator>::type parser_type;

  static bool call (Iterator& first, Iterator last, parser_type p, utree& ast) {
    typedef typename has_whitespace<Tag, Iterator>::type predicate;
    call(first, last, p, ast, predicate());
  }

  static bool call (Iterator& first, Iterator last, parser_type p,
                    utree& ast, mpl::false_) {
    using qi::parse;
    return parse(first, last, p, ast);
  }
  
  static bool call (Iterator& first, Iterator last, parser_type p,
                    utree& ast, mpl::true_) {
    using qi::phrase_parse;
    typedef typename whitespace_type<Tag, Iterator>::type ws_type;
    return phrase_parse(first, last, p, ws_type(), ast);
  }
};

template<class Tag, class Iterator>
bool invoke (Iterator& first, Iterator last,
             typename parser_type<Tag, Iterator>::type p, utree& ast)
{
  return parser_invoker<Tag, Iterator>::call(first, last, p, ast);
}

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
  typedef standard::space_type type;
}; 

template<class Tag, class Iterator, class Enable/* = void*/>
struct error_handler_type {
  typedef prana::error_handler<Tag, Iterator> type;
}; 

} // traits

namespace result_of {

template<class Tag, class Enable/* = void*/>
struct extract_source_location:
  traits::extract_source_location_from_node<Tag> { };

template<class Tag, class Enable/* = void*/>
struct extract_list_subtype:
  traits::extract_list_subtype_from_node<Tag> { };

} // result_of
} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_TRAITS_HPP


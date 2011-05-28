/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_C69F00A5_536F_48DD_9524_F282EE5432A9)
#define PRANA_C69F00A5_536F_48DD_9524_F282EE5432A9

#include <prana/include/utree.hpp>

#include <limits>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_container.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <prana/magic.hpp>
#include <prana/diagnostics/position_iterator.hpp>

namespace prana {

template<class Tag>
struct push_annotation {
  typedef typename magic::annotations_type<Tag>::type
    annotations_type;

  template<class, class>
  struct result {
    typedef void type;
  };

  annotations_type& annotations;

  push_annotation (annotations_type& annotations_):
    annotations(annotations_) { }

  template<class Range>
  void operator() (utree& ast, Range const& rng) const {
    typedef typename annotations_type::value_type value_type;
    typedef typename annotations_type::size_type size_type;
    
    value_type v(get_location(rng.begin()));

    annotations.push_back(v);
    size_type n = annotations.size() - 1;

    BOOST_ASSERT(n <= (std::numeric_limits<unsigned short>::max)());
    ast.tag(n);
  }
};

template<class Tag, class Iterator>
struct annotator: qi::grammar<Iterator, void(utree&)> {
  typedef typename magic::annotations_type<Tag>::type
    annotations_type;

  typedef push_annotation<Tag>
    pusher_type;

  qi::rule<Iterator, void(utree&)>
    start;

  phoenix::function<pusher_type> const
    push;

  annotator (annotations_type& annotations):
    annotator::base_type(start), push(pusher_type(annotations))
  {
    using qi::omit;
    using qi::raw;
    using qi::eps;
    using qi::_1;
    using qi::_r1;

    start = omit[raw[eps] [push(_r1, _1)]];
  }
};

} // prana

#endif // PRANA_C69F00A5_536F_48DD_9524_F282EE5432A9


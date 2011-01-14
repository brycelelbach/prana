/*==============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_SET_FWD_HPP)
#define BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_SET_FWD_HPP

#include <cstring>

#include <algorithm>

#include <boost/mpl/logical.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/remove_pointer.hpp>

#include <boost/range/iterator_range.hpp>
#include <boost/utility.hpp>

#include <boost/spirit/home/prana/dispatch/trampoline.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/traits_fwd.hpp>
#include <boost/spirit/home/prana/schematic/core_fwd.hpp>
#include <boost/spirit/home/prana/schematic/cast.hpp>
#include <boost/spirit/home/prana/schematic/routine/become.hpp>

namespace boost {
namespace spirit {
namespace prana {

//[set_visitable_declaration
template<class X, class Y>
inline typename enable_if<
  traits::is_visitable<Y>,
X&>::type set (X&, Y const&);
//]

//[set_boolean_declaration
template<class X>
inline X& set (X&, bool);

template<class X>
inline X& set (X&, mpl::true_);

template<class X>
inline X& set (X&, mpl::false_);
//]

//[set_ascii_declaration
template<class X>
inline X& set (X&, char);

template<class X>
inline X& set (X&, char const*);

template<class X>
inline X& set (X&, std::basic_string<char> const&);

template<class X, class Iterator>
inline X& set (X&, iterator_range<Iterator> const&);
//]

//[set_sentinel_type_declaration
template<class X>
inline X& set (X&, sentinel_type const&);

template<class X>
inline X& set (X&, spirit::unused_type const&);

template<class X>
inline X& set (X&, prana::unused_type const&);
//]

//[set_integer_declaration
template<class X, class Y>
inline typename enable_if<
  mpl::and_<
    is_integral<Y>,
    mpl::not_<
      is_same<Y, bool>
    >,
    mpl::not_<
      is_same<Y, char>
    >
  >,
X&>::type set (X&, Y const&);
//]

//[set_floating_declaration
template<class X, class Y>
inline typename enable_if<
  is_floating_point<Y>,
X&>::type set (X&, Y const&);
//]

namespace routine {

//[routine_set_declaration
template<class TagX, class TagY, class Dummy = void>
struct set;
//]

} // routine

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SCHEMATIC_ROUTINE_SET_FWD_HPP


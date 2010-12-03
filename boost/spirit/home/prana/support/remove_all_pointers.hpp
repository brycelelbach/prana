
/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SUPPORT_REMOVE_ALL_POINTERS_HPP)
#define BOOST_SPIRIT_PRANA_SUPPORT_REMOVE_ALL_POINTERS_HPP

#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/spirit/home/prana/domain.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class T, class Dummy = prana::unused_type>
struct remove_all_pointers;

template<class T, class Dummy>
struct remove_all_pointers:
  remove_pointer<T> { };

template<class T>
struct remove_all_pointers<
  T, typename enable_if<
    is_pointer<T>,
    prana::unused_type
  >::type
>:
  remove_all_pointers<typename remove_pointer<T>::type> { };

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SUPPORT_REMOVE_ALL_POINTERS_HPP ->*/

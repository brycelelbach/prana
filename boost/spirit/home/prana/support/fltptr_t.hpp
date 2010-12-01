/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SUPPORT_FLTPTR_T_HPP)
#define BOOST_SPIRIT_PRANA_SUPPORT_FLTPTR_T_HPP

#include <boost/mpl/if.hpp>

namespace boost {
namespace spirit {
namespace prana {

typedef mpl::if_c<
  sizeof(double) <= sizeof(void*), double, float
>::type fltptr_t;

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SUPPORT_FLTPTR_T_HPP ->*/


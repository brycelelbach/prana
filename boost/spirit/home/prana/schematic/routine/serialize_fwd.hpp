/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_C2213C5D_15C2_48A8_85F6_30FA8897837A)
#define BSP_C2213C5D_15C2_48A8_85F6_30FA8897837A

#include <boost/spirit/home/prana/dispatch/trampoline.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/magic.hpp>
#include <boost/spirit/home/prana/schematic/core_fwd.hpp>
#include <boost/spirit/home/prana/schematic/cast.hpp>

namespace boost {
namespace spirit {
namespace prana {

inline std::basic_string<char> serialize (schematic const&);

namespace routine {

template<class Tag, class Dummy = void>
struct serialize;

} // routine

} // prana
} // spirit
} // boost

#endif // BSP_C2213C5D_15C2_48A8_85F6_30FA8897837A


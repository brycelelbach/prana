/*<-============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_SERIALIZE_FWD_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_SERIALIZE_FWD_HPP

#include <boost/spirit/home/prana/trampoline.hpp>
#include <boost/spirit/home/prana/domain.hpp>
#include <boost/spirit/home/prana/sexpr/traits.hpp>
#include <boost/spirit/home/prana/sexpr/core_fwd.hpp>
#include <boost/spirit/home/prana/sexpr/cast.hpp>

#include <boost/spirit/include/karma.hpp> 

namespace boost {
namespace spirit {
namespace prana {

//[serialize_declaration
inline std::basic_string<char> serialize (sexpr const&);
//]

namespace routine {

//[routine_serialize_declaration
template<class Tag, class Dummy = prana::unused_type>
struct serialize;
//]

} /*<- routine ->*/

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_SEXPR_ROUTINE_SERIALIZE_FWD_HPP ->*/


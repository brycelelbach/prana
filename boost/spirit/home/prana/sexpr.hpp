/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SEXPR_HPP)
#define BOOST_SPIRIT_PRANA_SEXPR_HPP

#include <boost/config.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct nil_type { };

struct sexpr {
  std::size_t typeinfo;

  union atom {
    long      integer;
    double    floating;
    sexpr*    pair;
    void*     pointer;
    nil_type  nil;
  } car;

  sexpr* cdr;
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SEXPR_HPP

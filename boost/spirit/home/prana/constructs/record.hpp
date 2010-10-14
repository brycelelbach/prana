/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_RECORD_HPP)
#define BOOST_SPIRIT_PRANA_RECORD_HPP

#include <boost/spirit/home/prana/constructs/environment.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename Tree>
struct record {
 public:
  virtual ~record (void) { };
  virtual Tree operator() (environment<Tree> const& env) const = 0;
  virtual record* clone (void) const = 0;
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_RECORD_HPP

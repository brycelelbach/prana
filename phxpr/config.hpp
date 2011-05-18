////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2001-2011 Joel de Guzman
//  Copyright (c) 2001-2011 Hartmut Kaiser
//  Copyright (c) 2010-2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PHXPR_F8671203_734A_41A3_931F_A01CA87862EC)
#define PHXPR_F8671203_734A_41A3_931F_A01CA87862EC

// decltype-based result_of won't work with Spirit style CPs
#if defined(BOOST_RESULT_OF_USE_DECLTYPE)
  #undef BOOST_RESULT_OF_USE_DECLTYPE
#endif

#if !defined(PHXPR_ARITY_LIMIT)
  #define PHXPR_ARITY_LIMIT 8
#endif

#include <boost/config.hpp>

#include <phxpr/config/using.hpp>

#endif // PHXPR_F8671203_734A_41A3_931F_A01CA87862EC


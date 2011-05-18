////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010-2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PRANA_8C480E0F_D712_439D_8CF3_D542475E6F12)
#define PRANA_8C480E0F_D712_439D_8CF3_D542475E6F12

#include <sheol/config.hpp>

#define BOOST_SPIRIT_DISABLE_UTREE_IO

// Decltype-based result_of won't work with Spirit style CPs.
#if defined(BOOST_RESULT_OF_USE_DECLTYPE)
  #undef BOOST_RESULT_OF_USE_DECLTYPE
#endif

// For the time being, visit_ref can't use a nested result template because of
// utree limitations.
#define PRANA_NO_VISIT_REF_RESULT_TEMPLATE

// Don't use Karma grammars for stringifying utree instances, because we can't
// handle any_type, function_type and invalid_type utrees in the grammar yet.
#define PRANA_NO_UTREE_KARMA

#if !defined(PRANA_ARITY_LIMIT)
  #define PRANA_ARITY_LIMIT 8
#endif

#include <prana/config/using.hpp>

#endif // PRANA_8C480E0F_D712_439D_8CF3_D542475E6F12


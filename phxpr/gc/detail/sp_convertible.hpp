//  Copyright 2008 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef PHXPR_8D5D87EC_1DA8_4DE5_8A81_1D1253F507FF
#define PHXPR_8D5D87EC_1DA8_4DE5_8A81_1D1253F507FF

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <phxpr/config.hpp>

#if !defined( PHXPR_SP_NO_SP_CONVERTIBLE ) && defined( BOOST_NO_SFINAE )
# define PHXPR_SP_NO_SP_CONVERTIBLE
#endif

#if !defined( PHXPR_SP_NO_SP_CONVERTIBLE ) && defined( __GNUC__ ) && ( __GNUC__ * 100 + __GNUC_MINOR__ < 303 )
# define PHXPR_SP_NO_SP_CONVERTIBLE
#endif

#if !defined( PHXPR_SP_NO_SP_CONVERTIBLE ) && defined( __BORLANDC__ ) && ( __BORLANDC__ < 0x630 )
# define PHXPR_SP_NO_SP_CONVERTIBLE
#endif

#if !defined( PHXPR_SP_NO_SP_CONVERTIBLE )

namespace phxpr
{

namespace detail
{

template< class Y, class T > struct sp_convertible
{
    typedef char (&yes) [1];
    typedef char (&no)  [2];

    static yes f( T* );
    static no  f( ... );

    enum _vt { value = sizeof( (f)( static_cast<Y*>(0) ) ) == sizeof(yes) };
};

struct sp_empty
{
};

template< bool > struct sp_enable_if_convertible_impl;

template<> struct sp_enable_if_convertible_impl<true>
{
    typedef sp_empty type;
};

template<> struct sp_enable_if_convertible_impl<false>
{
};

template< class Y, class T > struct sp_enable_if_convertible: public sp_enable_if_convertible_impl< sp_convertible< Y, T >::value >
{
};

} // namespace detail

} // namespace phxpr

#endif // !defined( PHXPR_SP_NO_SP_CONVERTIBLE )

#endif  // #ifndef PHXPR_8D5D87EC_1DA8_4DE5_8A81_1D1253F507FF


//  Copyright (c) 2007, 2008 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//  See http://www.boost.org/libs/smart_ptr/make_shared.html
//  for documentation.

#ifndef PHXPR_B6B06596_1A19_4456_BC9D_583D0F6E33B6
#define PHXPR_B6B06596_1A19_4456_BC9D_583D0F6E33B6

#include <phxpr/config.hpp>

#include <cstddef>
#include <new>

#include <boost/type_traits/type_with_alignment.hpp>
#include <boost/type_traits/alignment_of.hpp>

#include <phxpr/gc/shared_ptr.hpp>

namespace phxpr
{

namespace detail
{

template< std::size_t N, std::size_t A > struct sp_aligned_storage
{
    union type
    {
        char data_[ N ];
        typename boost::type_with_alignment< A >::type align_;
    };
};

template< class T > class sp_ms_deleter
{
private:

    typedef typename sp_aligned_storage< sizeof( T ), ::boost::alignment_of< T >::value >::type storage_type;

    bool initialized_;
    storage_type storage_;

private:

    void destroy()
    {
        if( initialized_ )
        {
#if defined( __GNUC__ )

            // fixes incorrect aliasing warning
            T * p = reinterpret_cast< T* >( storage_.data_ );
            p->~T();

#else

            reinterpret_cast< T* >( storage_.data_ )->~T();

#endif

            initialized_ = false;
        }
    }

public:

    sp_ms_deleter(): initialized_( false )
    {
    }

    // optimization: do not copy storage_
    sp_ms_deleter( sp_ms_deleter const & ): initialized_( false )
    {
    }

    ~sp_ms_deleter()
    {
        destroy();
    }

    void operator()( T * )
    {
        destroy();
    }

    void * address()
    {
        return storage_.data_;
    }

    void set_initialized()
    {
        initialized_ = true;
    }
};

#if defined( BOOST_HAS_RVALUE_REFS )

template< class T > T&& sp_forward( T & t )
{
    return static_cast< T&& >( t );
}
      
#define PHXPR_SP_FORWARD(z, n, text) phxpr::detail::sp_forward<A##n>( a##n )

#endif

} // namespace detail

#if !defined( BOOST_NO_FUNCTION_TEMPLATE_ORDERING )
# define PHXPR_SP_MSD( T ) ::phxpr::detail::sp_inplace_tag< ::phxpr::detail::sp_ms_deleter< T > >()
#else
# define PHXPR_SP_MSD( T ) ::phxpr::detail::sp_ms_deleter< T >()
#endif

// Zero-argument versions
//
// Used even when variadic templates are available because of the new T() vs new T issue

template< class T > phxpr::shared_ptr< T > make_shared()
{
#if defined(PHXPR_SP_ENABLE_DEBUG_HOOKS)
    return phxpr::shared_ptr< T >(new T());
#else
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T();
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
#endif
}

template< class T, class A > phxpr::shared_ptr< T > allocate_shared( A const & a )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T();
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

#if defined( BOOST_HAS_VARIADIC_TMPL ) && defined( BOOST_HAS_RVALUE_REFS )

// Variadic templates, rvalue reference

template< class T, class Arg1, class... Args > phxpr::shared_ptr< T > make_shared( Arg1 && arg1, Args && ... args )
{
#if defined(PHXPR_SP_ENABLE_DEBUG_HOOKS)
    return phxpr::shared_ptr< T >(new T
        (phxpr::detail::sp_forward<Arg1>( arg1 ), phxpr::detail::sp_forward<Args>( args )... ));
#else
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( phxpr::detail::sp_forward<Arg1>( arg1 ), phxpr::detail::sp_forward<Args>( args )... );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
#endif
}

template< class T, class A, class Arg1, class... Args > phxpr::shared_ptr< T > allocate_shared( A const & a, Arg1 && arg1, Args && ... args )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( phxpr::detail::sp_forward<Arg1>( arg1 ), phxpr::detail::sp_forward<Args>( args )... );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

#else 

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#define BOOST_PP_ITERATION_PARAMS_1 (3, (1, PRANA_ARITY_LIMIT, <phxpr/gc/vertical/make_shared.hpp>))
#include BOOST_PP_ITERATE()

#endif

#undef PHXPR_SP_MSD

#if defined( BOOST_HAS_RVALUE_REFS )
    #undef PHXPR_SP_FORWARD
#endif

} // namespace phxpr

#endif // #ifndef PHXPR_B6B06596_1A19_4456_BC9D_583D0F6E33B6


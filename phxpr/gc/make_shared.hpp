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

template<class X>
class sp_counted_impl_pd<X*, sp_ms_deleter<X> >: public sp_counted_base
{
private:

    X* ptr; // copy constructor must not throw
    sp_ms_deleter<X> del; // copy constructor must not throw

    sp_counted_impl_pd( sp_counted_impl_pd const & );
    sp_counted_impl_pd & operator= ( sp_counted_impl_pd const & );

    typedef sp_counted_impl_pd<X*, sp_ms_deleter<X> > this_type;

public:

    // pre: d(p) must not throw

    sp_counted_impl_pd( X* p, sp_ms_deleter<X> & d ): ptr( p ), del( d )
    {
#if defined(PHXPR_SP_ENABLE_DEBUG_HOOKS)
        phxpr::sp_scalar_constructor_hook( p, sizeof(X), this );
#endif
    }

    sp_counted_impl_pd( X* p ): ptr( p ), del()
    {
#if defined(PHXPR_SP_ENABLE_DEBUG_HOOKS)
        phxpr::sp_scalar_constructor_hook( p, sizeof(X), this );
#endif
    }

    virtual void dispose() // nothrow
    {
#if defined(PHXPR_SP_ENABLE_DEBUG_HOOKS)
        phxpr::sp_scalar_destructor_hook( ptr, sizeof(X), this );
#endif
        del( ptr );
    }

    virtual void * get_deleter( boost::detail::sp_typeinfo const & ti )
    {
        return ti == BOOST_SP_TYPEID(sp_ms_deleter<X>)? &reinterpret_cast<char&>( del ): 0;
    }

#if defined(PHXPR_SP_USE_STD_ALLOCATOR)

    void * operator new( std::size_t )
    {
        return std::allocator<this_type>().allocate( 1, static_cast<this_type *>(0) );
    }

    void operator delete( void * p )
    {
        std::allocator<this_type>().deallocate( static_cast<this_type *>(p), 1 );
    }

#endif

#if defined(PHXPR_SP_USE_QUICK_ALLOCATOR)

    void * operator new( std::size_t )
    {
        return quick_allocator<this_type>::alloc();
    }

    void operator delete( void * p )
    {
        quick_allocator<this_type>::dealloc( p );
    }

#endif
};


#if defined( BOOST_HAS_RVALUE_REFS )

template< class T > T&& sp_forward( T & t )
{
    return static_cast< T&& >( t );
}

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
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T();
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
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
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( phxpr::detail::sp_forward<Arg1>( arg1 ), phxpr::detail::sp_forward<Args>( args )... );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
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

#elif defined( BOOST_HAS_RVALUE_REFS )

// For example MSVC 10.0

template< class T, class A1 >
phxpr::shared_ptr< T > make_shared( A1 && a1 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T(
        phxpr::detail::sp_forward<A1>( a1 )
        );

    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A, class A1 >
phxpr::shared_ptr< T > allocate_shared( A const & a, A1 && a1 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( 
        phxpr::detail::sp_forward<A1>( a1 )
        );

    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A1, class A2 >
phxpr::shared_ptr< T > make_shared( A1 && a1, A2 && a2 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T(
        phxpr::detail::sp_forward<A1>( a1 ), 
        phxpr::detail::sp_forward<A2>( a2 )
        );

    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A, class A1, class A2 >
phxpr::shared_ptr< T > allocate_shared( A const & a, A1 && a1, A2 && a2 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( 
        phxpr::detail::sp_forward<A1>( a1 ), 
        phxpr::detail::sp_forward<A2>( a2 )
        );

    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A1, class A2, class A3 >
phxpr::shared_ptr< T > make_shared( A1 && a1, A2 && a2, A3 && a3 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T(
        phxpr::detail::sp_forward<A1>( a1 ), 
        phxpr::detail::sp_forward<A2>( a2 ), 
        phxpr::detail::sp_forward<A3>( a3 )
        );

    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A, class A1, class A2, class A3 >
phxpr::shared_ptr< T > allocate_shared( A const & a, A1 && a1, A2 && a2, A3 && a3 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( 
        phxpr::detail::sp_forward<A1>( a1 ), 
        phxpr::detail::sp_forward<A2>( a2 ), 
        phxpr::detail::sp_forward<A3>( a3 )
        );

    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A1, class A2, class A3, class A4 >
phxpr::shared_ptr< T > make_shared( A1 && a1, A2 && a2, A3 && a3, A4 && a4 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T(
        phxpr::detail::sp_forward<A1>( a1 ), 
        phxpr::detail::sp_forward<A2>( a2 ), 
        phxpr::detail::sp_forward<A3>( a3 ), 
        phxpr::detail::sp_forward<A4>( a4 )
        );

    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A, class A1, class A2, class A3, class A4 >
phxpr::shared_ptr< T > allocate_shared( A const & a, A1 && a1, A2 && a2, A3 && a3, A4 && a4 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( 
        phxpr::detail::sp_forward<A1>( a1 ), 
        phxpr::detail::sp_forward<A2>( a2 ), 
        phxpr::detail::sp_forward<A3>( a3 ), 
        phxpr::detail::sp_forward<A4>( a4 )
        );

    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A1, class A2, class A3, class A4, class A5 >
phxpr::shared_ptr< T > make_shared( A1 && a1, A2 && a2, A3 && a3, A4 && a4, A5 && a5 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T(
        phxpr::detail::sp_forward<A1>( a1 ), 
        phxpr::detail::sp_forward<A2>( a2 ), 
        phxpr::detail::sp_forward<A3>( a3 ), 
        phxpr::detail::sp_forward<A4>( a4 ), 
        phxpr::detail::sp_forward<A5>( a5 )
        );

    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A, class A1, class A2, class A3, class A4, class A5 >
phxpr::shared_ptr< T > allocate_shared( A const & a, A1 && a1, A2 && a2, A3 && a3, A4 && a4, A5 && a5 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( 
        phxpr::detail::sp_forward<A1>( a1 ), 
        phxpr::detail::sp_forward<A2>( a2 ), 
        phxpr::detail::sp_forward<A3>( a3 ), 
        phxpr::detail::sp_forward<A4>( a4 ), 
        phxpr::detail::sp_forward<A5>( a5 )
        );

    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A1, class A2, class A3, class A4, class A5, class A6 >
phxpr::shared_ptr< T > make_shared( A1 && a1, A2 && a2, A3 && a3, A4 && a4, A5 && a5, A6 && a6 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T(
        phxpr::detail::sp_forward<A1>( a1 ), 
        phxpr::detail::sp_forward<A2>( a2 ), 
        phxpr::detail::sp_forward<A3>( a3 ), 
        phxpr::detail::sp_forward<A4>( a4 ), 
        phxpr::detail::sp_forward<A5>( a5 ), 
        phxpr::detail::sp_forward<A6>( a6 )
        );

    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A, class A1, class A2, class A3, class A4, class A5, class A6 >
phxpr::shared_ptr< T > allocate_shared( A const & a, A1 && a1, A2 && a2, A3 && a3, A4 && a4, A5 && a5, A6 && a6 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( 
        phxpr::detail::sp_forward<A1>( a1 ), 
        phxpr::detail::sp_forward<A2>( a2 ), 
        phxpr::detail::sp_forward<A3>( a3 ), 
        phxpr::detail::sp_forward<A4>( a4 ), 
        phxpr::detail::sp_forward<A5>( a5 ), 
        phxpr::detail::sp_forward<A6>( a6 )
        );

    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7 >
phxpr::shared_ptr< T > make_shared( A1 && a1, A2 && a2, A3 && a3, A4 && a4, A5 && a5, A6 && a6, A7 && a7 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T(
        phxpr::detail::sp_forward<A1>( a1 ), 
        phxpr::detail::sp_forward<A2>( a2 ), 
        phxpr::detail::sp_forward<A3>( a3 ), 
        phxpr::detail::sp_forward<A4>( a4 ), 
        phxpr::detail::sp_forward<A5>( a5 ), 
        phxpr::detail::sp_forward<A6>( a6 ), 
        phxpr::detail::sp_forward<A7>( a7 )
        );

    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A, class A1, class A2, class A3, class A4, class A5, class A6, class A7 >
phxpr::shared_ptr< T > allocate_shared( A const & a, A1 && a1, A2 && a2, A3 && a3, A4 && a4, A5 && a5, A6 && a6, A7 && a7 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( 
        phxpr::detail::sp_forward<A1>( a1 ), 
        phxpr::detail::sp_forward<A2>( a2 ), 
        phxpr::detail::sp_forward<A3>( a3 ), 
        phxpr::detail::sp_forward<A4>( a4 ), 
        phxpr::detail::sp_forward<A5>( a5 ), 
        phxpr::detail::sp_forward<A6>( a6 ), 
        phxpr::detail::sp_forward<A7>( a7 )
        );

    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8 >
phxpr::shared_ptr< T > make_shared( A1 && a1, A2 && a2, A3 && a3, A4 && a4, A5 && a5, A6 && a6, A7 && a7, A8 && a8 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T(
        phxpr::detail::sp_forward<A1>( a1 ), 
        phxpr::detail::sp_forward<A2>( a2 ), 
        phxpr::detail::sp_forward<A3>( a3 ), 
        phxpr::detail::sp_forward<A4>( a4 ), 
        phxpr::detail::sp_forward<A5>( a5 ), 
        phxpr::detail::sp_forward<A6>( a6 ), 
        phxpr::detail::sp_forward<A7>( a7 ), 
        phxpr::detail::sp_forward<A8>( a8 )
        );

    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8 >
phxpr::shared_ptr< T > allocate_shared( A const & a, A1 && a1, A2 && a2, A3 && a3, A4 && a4, A5 && a5, A6 && a6, A7 && a7, A8 && a8 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( 
        phxpr::detail::sp_forward<A1>( a1 ), 
        phxpr::detail::sp_forward<A2>( a2 ), 
        phxpr::detail::sp_forward<A3>( a3 ), 
        phxpr::detail::sp_forward<A4>( a4 ), 
        phxpr::detail::sp_forward<A5>( a5 ), 
        phxpr::detail::sp_forward<A6>( a6 ), 
        phxpr::detail::sp_forward<A7>( a7 ), 
        phxpr::detail::sp_forward<A8>( a8 )
        );

    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9 >
phxpr::shared_ptr< T > make_shared( A1 && a1, A2 && a2, A3 && a3, A4 && a4, A5 && a5, A6 && a6, A7 && a7, A8 && a8, A9 && a9 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T(
        phxpr::detail::sp_forward<A1>( a1 ), 
        phxpr::detail::sp_forward<A2>( a2 ), 
        phxpr::detail::sp_forward<A3>( a3 ), 
        phxpr::detail::sp_forward<A4>( a4 ), 
        phxpr::detail::sp_forward<A5>( a5 ), 
        phxpr::detail::sp_forward<A6>( a6 ), 
        phxpr::detail::sp_forward<A7>( a7 ), 
        phxpr::detail::sp_forward<A8>( a8 ), 
        phxpr::detail::sp_forward<A9>( a9 )
        );

    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9 >
phxpr::shared_ptr< T > allocate_shared( A const & a, A1 && a1, A2 && a2, A3 && a3, A4 && a4, A5 && a5, A6 && a6, A7 && a7, A8 && a8, A9 && a9 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( 
        phxpr::detail::sp_forward<A1>( a1 ), 
        phxpr::detail::sp_forward<A2>( a2 ), 
        phxpr::detail::sp_forward<A3>( a3 ), 
        phxpr::detail::sp_forward<A4>( a4 ), 
        phxpr::detail::sp_forward<A5>( a5 ), 
        phxpr::detail::sp_forward<A6>( a6 ), 
        phxpr::detail::sp_forward<A7>( a7 ), 
        phxpr::detail::sp_forward<A8>( a8 ), 
        phxpr::detail::sp_forward<A9>( a9 )
        );

    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

#else

// C++03 version

template< class T, class A1 >
phxpr::shared_ptr< T > make_shared( A1 const & a1 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( a1 );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A, class A1 >
phxpr::shared_ptr< T > allocate_shared( A const & a, A1 const & a1 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( a1 );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A1, class A2 >
phxpr::shared_ptr< T > make_shared( A1 const & a1, A2 const & a2 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( a1, a2 );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A, class A1, class A2 >
phxpr::shared_ptr< T > allocate_shared( A const & a, A1 const & a1, A2 const & a2 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( a1, a2 );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A1, class A2, class A3 >
phxpr::shared_ptr< T > make_shared( A1 const & a1, A2 const & a2, A3 const & a3 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( a1, a2, a3 );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A, class A1, class A2, class A3 >
phxpr::shared_ptr< T > allocate_shared( A const & a, A1 const & a1, A2 const & a2, A3 const & a3 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( a1, a2, a3 );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A1, class A2, class A3, class A4 >
phxpr::shared_ptr< T > make_shared( A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( a1, a2, a3, a4 );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A, class A1, class A2, class A3, class A4 >
phxpr::shared_ptr< T > allocate_shared( A const & a, A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( a1, a2, a3, a4 );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A1, class A2, class A3, class A4, class A5 >
phxpr::shared_ptr< T > make_shared( A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4, A5 const & a5 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( a1, a2, a3, a4, a5 );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A, class A1, class A2, class A3, class A4, class A5 >
phxpr::shared_ptr< T > allocate_shared( A const & a, A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4, A5 const & a5 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( a1, a2, a3, a4, a5 );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A1, class A2, class A3, class A4, class A5, class A6 >
phxpr::shared_ptr< T > make_shared( A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4, A5 const & a5, A6 const & a6 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( a1, a2, a3, a4, a5, a6 );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A, class A1, class A2, class A3, class A4, class A5, class A6 >
phxpr::shared_ptr< T > allocate_shared( A const & a, A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4, A5 const & a5, A6 const & a6 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( a1, a2, a3, a4, a5, a6 );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7 >
phxpr::shared_ptr< T > make_shared( A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4, A5 const & a5, A6 const & a6, A7 const & a7 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( a1, a2, a3, a4, a5, a6, a7 );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A, class A1, class A2, class A3, class A4, class A5, class A6, class A7 >
phxpr::shared_ptr< T > allocate_shared( A const & a, A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4, A5 const & a5, A6 const & a6, A7 const & a7 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( a1, a2, a3, a4, a5, a6, a7 );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8 >
phxpr::shared_ptr< T > make_shared( A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4, A5 const & a5, A6 const & a6, A7 const & a7, A8 const & a8 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( a1, a2, a3, a4, a5, a6, a7, a8 );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8 >
phxpr::shared_ptr< T > allocate_shared( A const & a, A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4, A5 const & a5, A6 const & a6, A7 const & a7, A8 const & a8 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( a1, a2, a3, a4, a5, a6, a7, a8 );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9 >
phxpr::shared_ptr< T > make_shared( A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4, A5 const & a5, A6 const & a6, A7 const & a7, A8 const & a8, A9 const & a9 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( a1, a2, a3, a4, a5, a6, a7, a8, a9 );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

template< class T, class A, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9 >
phxpr::shared_ptr< T > allocate_shared( A const & a, A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4, A5 const & a5, A6 const & a6, A7 const & a7, A8 const & a8, A9 const & a9 )
{
    phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );

    phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( a1, a2, a3, a4, a5, a6, a7, a8, a9 );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return phxpr::shared_ptr< T >( pt, pt2 );
}

#endif

#undef PHXPR_SP_MSD

} // namespace phxpr

#endif // #ifndef PHXPR_B6B06596_1A19_4456_BC9D_583D0F6E33B6


//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
//  Copyright (c) 2001-2008 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/smart_ptr/shared_ptr.htm for documentation.

#ifndef PHXPR_C5840A0F_551F_455C_889D_E038AFA968C8
#define PHXPR_C5840A0F_551F_455C_889D_E038AFA968C8

#include <phxpr/config.hpp>   // for broken compiler workarounds

// In order to avoid circular dependencies with Boost.TR1
// we make sure that our include of <memory> doesn't try to
// pull in the TR1 headers: that's why we use this header 
// rather than including <memory> directly:
#include <boost/config/no_tr1/memory.hpp>  // std::auto_ptr

#include <boost/assert.hpp>
#include <boost/checked_delete.hpp>
#include <boost/throw_exception.hpp>
#include <boost/detail/workaround.hpp>

#include <phxpr/gc/detail/shared_count.hpp>
#include <phxpr/gc/detail/sp_convertible.hpp>

#include <algorithm>            // for std::swap
#include <functional>           // for std::less
#include <typeinfo>             // for std::bad_cast
#include <cstddef>              // for std::size_t

#if !defined(BOOST_NO_IOSTREAM)
#if !defined(BOOST_NO_IOSFWD)
#include <iosfwd>               // for std::basic_ostream
#else
#include <ostream>
#endif
#endif

namespace phxpr
{

template<class T> class shared_ptr;
template<class T> class weak_ptr;
template<class T> class enable_shared_from_this;
class enable_shared_from_raw;

namespace detail
{

struct static_cast_tag {};
struct const_cast_tag {};
struct dynamic_cast_tag {};
struct polymorphic_cast_tag {};

template<class T> struct shared_ptr_traits
{
    typedef T & reference;
};

template<> struct shared_ptr_traits<void>
{
    typedef void reference;
};

#if !defined(BOOST_NO_CV_VOID_SPECIALIZATIONS)

template<> struct shared_ptr_traits<void const>
{
    typedef void reference;
};

template<> struct shared_ptr_traits<void volatile>
{
    typedef void reference;
};

template<> struct shared_ptr_traits<void const volatile>
{
    typedef void reference;
};

#endif

// enable_shared_from_this support

template< class X, class Y, class T > inline void sp_enable_shared_from_this( phxpr::shared_ptr<X> const * ppx, Y const * py, phxpr::enable_shared_from_this< T > const * pe )
{
    if( pe != 0 )
    {
        pe->_internal_accept_owner( ppx, const_cast< Y* >( py ) );
    }
}

template< class X, class Y > inline void sp_enable_shared_from_this( phxpr::shared_ptr<X> * ppx, Y const * py, phxpr::enable_shared_from_raw const * pe );

#ifdef _MANAGED

// Avoid C4793, ... causes native code generation

struct sp_any_pointer
{
    template<class T> sp_any_pointer( T* ) {}
};

inline void sp_enable_shared_from_this( sp_any_pointer, sp_any_pointer, sp_any_pointer )
{
}

#else // _MANAGED

inline void sp_enable_shared_from_this( ... )
{
}

#endif // _MANAGED

#if !defined( BOOST_NO_SFINAE ) && !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION ) && !defined( BOOST_NO_AUTO_PTR )

// rvalue auto_ptr support based on a technique by Dave Abrahams

template< class T, class R > struct sp_enable_if_auto_ptr
{
};

template< class T, class R > struct sp_enable_if_auto_ptr< std::auto_ptr< T >, R >
{
    typedef R type;
}; 

#endif

} // namespace detail


//
//  shared_ptr
//
//  An enhanced relative of scoped_ptr with reference counted copy semantics.
//  The object pointed to is deleted when the last shared_ptr pointing to it
//  is destroyed or reset.
//

template<class T> class shared_ptr
{
private:

    // Borland 5.5.1 specific workaround
    typedef shared_ptr<T> this_type;

public:

    typedef T element_type;
    typedef T value_type;
    typedef T * pointer;
    typedef typename phxpr::detail::shared_ptr_traits<T>::reference reference;

    shared_ptr(): px(0), pn() // never throws in 1.30+
    {
    }

    template<class Y>
    explicit shared_ptr( Y * p ): px( p ), pn( p ) // Y must be complete
    {
        phxpr::detail::sp_enable_shared_from_this( this, p, p );
    }

    //
    // Requirements: D's copy constructor must not throw
    //
    // shared_ptr will release p by calling d(p)
    //

    template<class Y, class D> shared_ptr(Y * p, D d): px(p), pn(p, d)
    {
        phxpr::detail::sp_enable_shared_from_this( this, p, p );
    }

    // As above, but with allocator. A's copy constructor shall not throw.

    template<class Y, class D, class A> shared_ptr( Y * p, D d, A a ): px( p ), pn( p, d, a )
    {
        phxpr::detail::sp_enable_shared_from_this( this, p, p );
    }

//  generated copy constructor, destructor are fine

    template<class Y>
    explicit shared_ptr(weak_ptr<Y> const & r): pn(r.pn) // may throw
    {
        // it is now safe to copy r.px, as pn(r.pn) did not throw
        px = r.px;
    }

    template<class Y>
    shared_ptr( weak_ptr<Y> const & r, phxpr::detail::sp_nothrow_tag ): px( 0 ), pn( r.pn, phxpr::detail::sp_nothrow_tag() ) // never throws
    {
        if( !pn.empty() )
        {
            px = r.px;
        }
    }

    template<class Y>
#if !defined( PHXPR_SP_NO_SP_CONVERTIBLE )

    shared_ptr( shared_ptr<Y> const & r, typename phxpr::detail::sp_enable_if_convertible<Y,T>::type = phxpr::detail::sp_empty() )

#else

    shared_ptr( shared_ptr<Y> const & r )

#endif
    : px( r.px ), pn( r.pn ) // never throws
    {
    }

    // aliasing
    template< class Y >
    shared_ptr( shared_ptr<Y> const & r, T * p ): px( p ), pn( r.pn ) // never throws
    {
    }

    template<class Y>
    shared_ptr(shared_ptr<Y> const & r, phxpr::detail::static_cast_tag): px(static_cast<element_type *>(r.px)), pn(r.pn)
    {
    }

    template<class Y>
    shared_ptr(shared_ptr<Y> const & r, phxpr::detail::const_cast_tag): px(const_cast<element_type *>(r.px)), pn(r.pn)
    {
    }

    template<class Y>
    shared_ptr(shared_ptr<Y> const & r, phxpr::detail::dynamic_cast_tag): px(dynamic_cast<element_type *>(r.px)), pn(r.pn)
    {
        if(px == 0) // need to allocate new counter -- the cast failed
        {
            pn = phxpr::detail::shared_count();
        }
    }

    template<class Y>
    shared_ptr(shared_ptr<Y> const & r, phxpr::detail::polymorphic_cast_tag): px(dynamic_cast<element_type *>(r.px)), pn(r.pn)
    {
        if(px == 0)
        {
            boost::throw_exception(std::bad_cast());
        }
    }

#ifndef BOOST_NO_AUTO_PTR

    template<class Y>
    explicit shared_ptr(std::auto_ptr<Y> & r): px(r.get()), pn()
    {
        Y * tmp = r.get();
        pn = phxpr::detail::shared_count(r);
        phxpr::detail::sp_enable_shared_from_this( this, tmp, tmp );
    }

#if !defined( BOOST_NO_SFINAE ) && !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

    template<class Ap>
    explicit shared_ptr( Ap r, typename phxpr::detail::sp_enable_if_auto_ptr<Ap, int>::type = 0 ): px( r.get() ), pn()
    {
        typename Ap::element_type * tmp = r.get();
        pn = phxpr::detail::shared_count( r );
        phxpr::detail::sp_enable_shared_from_this( this, tmp, tmp );
    }


#endif // BOOST_NO_SFINAE, BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // BOOST_NO_AUTO_PTR

    // assignment

    shared_ptr & operator=( shared_ptr const & r ) // never throws
    {
        this_type(r).swap(*this);
        return *this;
    }

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1400)

    template<class Y>
    shared_ptr & operator=(shared_ptr<Y> const & r) // never throws
    {
        this_type(r).swap(*this);
        return *this;
    }

#endif

#ifndef BOOST_NO_AUTO_PTR

    template<class Y>
    shared_ptr & operator=( std::auto_ptr<Y> & r )
    {
        this_type(r).swap(*this);
        return *this;
    }

#if !defined( BOOST_NO_SFINAE ) && !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

    template<class Ap>
    typename phxpr::detail::sp_enable_if_auto_ptr< Ap, shared_ptr & >::type operator=( Ap r )
    {
        this_type( r ).swap( *this );
        return *this;
    }


#endif // BOOST_NO_SFINAE, BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // BOOST_NO_AUTO_PTR

// Move support

#if defined( BOOST_HAS_RVALUE_REFS )

    shared_ptr( shared_ptr && r ): px( r.px ), pn() // never throws
    {
        pn.swap( r.pn );
        r.px = 0;
    }

    template<class Y>
#if !defined( PHXPR_SP_NO_SP_CONVERTIBLE )

    shared_ptr( shared_ptr<Y> && r, typename phxpr::detail::sp_enable_if_convertible<Y,T>::type = phxpr::detail::sp_empty() )

#else

    shared_ptr( shared_ptr<Y> && r )

#endif
    : px( r.px ), pn() // never throws
    {
        pn.swap( r.pn );
        r.px = 0;
    }

    shared_ptr & operator=( shared_ptr && r ) // never throws
    {
        this_type( static_cast< shared_ptr && >( r ) ).swap( *this );
        return *this;
    }

    template<class Y>
    shared_ptr & operator=( shared_ptr<Y> && r ) // never throws
    {
        this_type( static_cast< shared_ptr<Y> && >( r ) ).swap( *this );
        return *this;
    }

#endif

    void reset() // never throws in 1.30+
    {
        this_type().swap(*this);
    }

    template<class Y> void reset(Y * p) // Y must be complete
    {
        BOOST_ASSERT(p == 0 || p != px); // catch self-reset errors
        this_type(p).swap(*this);
    }

    template<class Y, class D> void reset( Y * p, D d )
    {
        this_type( p, d ).swap( *this );
    }

    template<class Y, class D, class A> void reset( Y * p, D d, A a )
    {
        this_type( p, d, a ).swap( *this );
    }

    template<class Y> void reset( shared_ptr<Y> const & r, T * p )
    {
        this_type( r, p ).swap( *this );
    }

    reference operator* () const // never throws
    {
        BOOST_ASSERT(px != 0);
        return *px;
    }

    T * operator-> () const // never throws
    {
        BOOST_ASSERT(px != 0);
        return px;
    }

    T * get() const // never throws
    {
        return px;
    }

// implicit conversion to "bool"
#include <phxpr/gc/detail/operator_bool.hpp>

    bool unique() const // never throws
    {
        return pn.unique();
    }

    long use_count() const // never throws
    {
        return pn.use_count();
    }

    void swap(shared_ptr<T> & other) // never throws
    {
        std::swap(px, other.px);
        pn.swap(other.pn);
    }

    template<class Y> bool _internal_less(shared_ptr<Y> const & rhs) const
    {
        return pn < rhs.pn;
    }

    void * _internal_get_deleter( boost::detail::sp_typeinfo const & ti ) const
    {
        return pn.get_deleter( ti );
    }

    bool _internal_equiv( shared_ptr const & r ) const
    {
        return px == r.px && pn == r.pn;
    }

// Tasteless as this may seem, making all members public allows member templates
// to work in the absence of member template friends. (Matthew Langston)

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS

private:

    template<class Y> friend class shared_ptr;
    template<class Y> friend class weak_ptr;


#endif

    T * px;                     // contained pointer
    phxpr::detail::shared_count pn;    // reference counter

};  // shared_ptr

template<class T, class U> inline bool operator==(shared_ptr<T> const & a, shared_ptr<U> const & b)
{
    return a.get() == b.get();
}

template<class T, class U> inline bool operator!=(shared_ptr<T> const & a, shared_ptr<U> const & b)
{
    return a.get() != b.get();
}

#if __GNUC__ == 2 && __GNUC_MINOR__ <= 96

// Resolve the ambiguity between our op!= and the one in rel_ops

template<class T> inline bool operator!=(shared_ptr<T> const & a, shared_ptr<T> const & b)
{
    return a.get() != b.get();
}

#endif

template<class T, class U> inline bool operator<(shared_ptr<T> const & a, shared_ptr<U> const & b)
{
    return a._internal_less(b);
}

template<class T> inline void swap(shared_ptr<T> & a, shared_ptr<T> & b)
{
    a.swap(b);
}

template<class T, class U> shared_ptr<T> static_pointer_cast(shared_ptr<U> const & r)
{
    return shared_ptr<T>(r, phxpr::detail::static_cast_tag());
}

template<class T, class U> shared_ptr<T> const_pointer_cast(shared_ptr<U> const & r)
{
    return shared_ptr<T>(r, phxpr::detail::const_cast_tag());
}

template<class T, class U> shared_ptr<T> dynamic_pointer_cast(shared_ptr<U> const & r)
{
    return shared_ptr<T>(r, phxpr::detail::dynamic_cast_tag());
}

// shared_*_cast names are deprecated. Use *_pointer_cast instead.

template<class T, class U> shared_ptr<T> shared_static_cast(shared_ptr<U> const & r)
{
    return shared_ptr<T>(r, phxpr::detail::static_cast_tag());
}

template<class T, class U> shared_ptr<T> shared_dynamic_cast(shared_ptr<U> const & r)
{
    return shared_ptr<T>(r, phxpr::detail::dynamic_cast_tag());
}

template<class T, class U> shared_ptr<T> shared_polymorphic_cast(shared_ptr<U> const & r)
{
    return shared_ptr<T>(r, phxpr::detail::polymorphic_cast_tag());
}

template<class T, class U> shared_ptr<T> shared_polymorphic_downcast(shared_ptr<U> const & r)
{
    BOOST_ASSERT(dynamic_cast<T *>(r.get()) == r.get());
    return shared_static_cast<T>(r);
}

// get_pointer() enables boost::mem_fn to recognize shared_ptr

template<class T> inline T * get_pointer(shared_ptr<T> const & p)
{
    return p.get();
}

// operator<<

#if !defined(BOOST_NO_IOSTREAM)

#if defined(BOOST_NO_TEMPLATED_IOSTREAMS) || ( defined(__GNUC__) &&  (__GNUC__ < 3) )

template<class Y> std::ostream & operator<< (std::ostream & os, shared_ptr<Y> const & p)
{
    os << p.get();
    return os;
}

#else

// in STLport's no-iostreams mode no iostream symbols can be used
#ifndef _STLP_NO_IOSTREAMS

# if defined(BOOST_MSVC) && BOOST_WORKAROUND(BOOST_MSVC, < 1300 && __SGI_STL_PORT)
// MSVC6 has problems finding std::basic_ostream through the using declaration in namespace _STL
using std::basic_ostream;
template<class E, class T, class Y> basic_ostream<E, T> & operator<< (basic_ostream<E, T> & os, shared_ptr<Y> const & p)
# else
template<class E, class T, class Y> std::basic_ostream<E, T> & operator<< (std::basic_ostream<E, T> & os, shared_ptr<Y> const & p)
# endif
{
    os << p.get();
    return os;
}

#endif // _STLP_NO_IOSTREAMS

#endif // __GNUC__ < 3

#endif // !defined(BOOST_NO_IOSTREAM)

// get_deleter

namespace detail
{

#if ( defined(__GNUC__) && BOOST_WORKAROUND(__GNUC__, < 3) ) || \
    ( defined(__EDG_VERSION__) && BOOST_WORKAROUND(__EDG_VERSION__, <= 238) ) || \
    ( defined(__HP_aCC) && BOOST_WORKAROUND(__HP_aCC, <= 33500) )

// g++ 2.9x doesn't allow static_cast<X const *>(void *)
// apparently EDG 2.38 and HP aCC A.03.35 also don't accept it

template<class D, class T> D * basic_get_deleter(shared_ptr<T> const & p)
{
    void const * q = p._internal_get_deleter(BOOST_SP_TYPEID(D));
    return const_cast<D *>(static_cast<D const *>(q));
}

#else

template<class D, class T> D * basic_get_deleter(shared_ptr<T> const & p)
{
    return static_cast<D *>(p._internal_get_deleter(BOOST_SP_TYPEID(D)));
}

#endif

class esft2_deleter_wrapper
{
private:

    shared_ptr<void> deleter_;

public:

    esft2_deleter_wrapper()
    {
    }

    template< class T > void set_deleter( shared_ptr<T> const & deleter )
    {
        deleter_ = deleter;
    }
    template<typename D> D* get_deleter() const
    {
        return phxpr::detail::basic_get_deleter<D>(deleter_);
    }
    template< class T> void operator()( T* )
    {
        BOOST_ASSERT( deleter_.use_count() <= 1 );
        deleter_.reset();
    }
};

} // namespace detail

template<class D, class T> D * get_deleter(shared_ptr<T> const & p)
{
    D *del = phxpr::detail::basic_get_deleter<D>(p);
    if(del == 0)
    {
        phxpr::detail::esft2_deleter_wrapper *del_wrapper = phxpr::detail::basic_get_deleter<phxpr::detail::esft2_deleter_wrapper>(p);
// The following get_deleter method call is fully qualified because
// older versions of gcc (2.95, 3.2.3) fail to compile it when written del_wrapper->get_deleter<D>()
        if(del_wrapper) del = del_wrapper->::phxpr::detail::esft2_deleter_wrapper::get_deleter<D>();
    }
    return del;
}

// hash_value

} // phxpr

namespace boost {

template< class T > struct hash;

} // boost

namespace phxpr {

template< class T > std::size_t hash_value( phxpr::shared_ptr<T> const & p )
{
    return boost::hash< T* >()( p.get() );
}

} // namespace phxpr

#endif  // #ifndef PHXPR_C5840A0F_551F_455C_889D_E038AFA968C8


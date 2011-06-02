//  Copyright (c) 2001, 2002, 2003 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/smart_ptr/weak_ptr.htm for documentation.

#ifndef PHXPR_285EB497_F76F_4B19_9AD2_AE1A9A976BDD
#define PHXPR_285EB497_F76F_4B19_9AD2_AE1A9A976BDD

#include <memory> // boost.TR1 include order fix

#include <phxpr/gc/detail/shared_count.hpp>
#include <phxpr/gc/shared_ptr.hpp>

#ifdef BOOST_MSVC  // moved here to work around VC++ compiler crash
# pragma warning(push)
# pragma warning(disable:4284) // odd return type for operator->
#endif

namespace phxpr
{

template<class T> class weak_ptr
{
private:

    // Borland 5.5.1 specific workarounds
    typedef weak_ptr<T> this_type;

public:

    typedef T element_type;

    weak_ptr(): px(0), pn() // never throws in 1.30+
    {
    }

//  generated copy constructor, assignment, destructor are fine


//
//  The "obvious" converting constructor implementation:
//
//  template<class Y>
//  weak_ptr(weak_ptr<Y> const & r): px(r.px), pn(r.pn) // never throws
//  {
//  }
//
//  has a serious problem.
//
//  r.px may already have been invalidated. The px(r.px)
//  conversion may require access to *r.px (virtual inheritance).
//
//  It is not possible to avoid spurious access violations since
//  in multithreaded programs r.px may be invalidated at any point.
//

    template<class Y>
#if !defined( PHXPR_SP_NO_SP_CONVERTIBLE )

    weak_ptr( weak_ptr<Y> const & r, typename phxpr::detail::sp_enable_if_convertible<Y,T>::type = phxpr::detail::sp_empty() )

#else

    weak_ptr( weak_ptr<Y> const & r )

#endif
    : px(r.lock().get()), pn(r.pn) // never throws
    {
    }

#if defined( BOOST_HAS_RVALUE_REFS )

    template<class Y>
#if !defined( PHXPR_SP_NO_SP_CONVERTIBLE )

    weak_ptr( weak_ptr<Y> && r, typename phxpr::detail::sp_enable_if_convertible<Y,T>::type = phxpr::detail::sp_empty() )

#else

    weak_ptr( weak_ptr<Y> && r )

#endif
    : px( r.lock().get() ), pn( static_cast< phxpr::detail::weak_count && >( r.pn ) ) // never throws
    {
        r.px = 0;
    }

    // for better efficiency in the T == Y case
    weak_ptr( weak_ptr && r ): px( r.px ), pn( static_cast< phxpr::detail::weak_count && >( r.pn ) ) // never throws
    {
        r.px = 0;
    }

    // for better efficiency in the T == Y case
    weak_ptr & operator=( weak_ptr && r ) // never throws
    {
        this_type( static_cast< weak_ptr && >( r ) ).swap( *this );
        return *this;
    }


#endif

    template<class Y>
#if !defined( PHXPR_SP_NO_SP_CONVERTIBLE )

    weak_ptr( shared_ptr<Y> const & r, typename phxpr::detail::sp_enable_if_convertible<Y,T>::type = phxpr::detail::sp_empty() )

#else

    weak_ptr( shared_ptr<Y> const & r )

#endif
    : px( r.px ), pn( r.pn ) // never throws
    {
    }

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1300)

    template<class Y>
    weak_ptr & operator=(weak_ptr<Y> const & r) // never throws
    {
        px = r.lock().get();
        pn = r.pn;
        return *this;
    }

#if defined( BOOST_HAS_RVALUE_REFS )

    template<class Y>
    weak_ptr & operator=( weak_ptr<Y> && r )
    {
        this_type( static_cast< weak_ptr<Y> && >( r ) ).swap( *this );
        return *this;
    }

#endif

    template<class Y>
    weak_ptr & operator=(shared_ptr<Y> const & r) // never throws
    {
        px = r.px;
        pn = r.pn;
        return *this;
    }

#endif

    shared_ptr<T> lock() const // never throws
    {
        return shared_ptr<element_type>( *this, phxpr::detail::sp_nothrow_tag() );
    }

    long use_count() const // never throws
    {
        return pn.use_count();
    }

    bool expired() const // never throws
    {
        return pn.use_count() == 0;
    }

    bool _empty() const // extension, not in std::weak_ptr
    {
        return pn.empty();
    }

    void reset() // never throws in 1.30+
    {
        this_type().swap(*this);
    }

    void swap(this_type & other) // never throws
    {
        std::swap(px, other.px);
        pn.swap(other.pn);
    }

    template<typename Y>
    void _internal_aliasing_assign(weak_ptr<Y> const & r, T * px2)
    {
        px = px2;
        pn = r.pn;
    }

    template<class Y> bool _internal_less(weak_ptr<Y> const & rhs) const
    {
        return pn < rhs.pn;
    }

// Tasteless as this may seem, making all members public allows member templates
// to work in the absence of member template friends. (Matthew Langston)

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS

private:

    template<class Y> friend class weak_ptr;
    template<class Y> friend class shared_ptr;

#endif

    T * px;                       // contained pointer
    phxpr::detail::weak_count pn; // reference counter

};  // weak_ptr

template<class T, class U> inline bool operator<(weak_ptr<T> const & a, weak_ptr<U> const & b)
{
    return a._internal_less(b);
}

template<class T> void swap(weak_ptr<T> & a, weak_ptr<T> & b)
{
    a.swap(b);
}

} // namespace phxpr

#ifdef BOOST_MSVC
# pragma warning(pop)
#endif

#endif  // #ifndef PHXPR_285EB497_F76F_4B19_9AD2_AE1A9A976BDD


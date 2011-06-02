//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
//  Copyright (c) 2001, 2002 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/smart_ptr/shared_array.htm for documentation.

#ifndef PHXPR_A247A4AD_6551_49D0_8F05_CA4A5AA975C0
#define PHXPR_A247A4AD_6551_49D0_8F05_CA4A5AA975C0

#include <phxpr/config.hpp>   // for broken compiler workarounds

#include <memory>             // TR1 cyclic inclusion fix
#include <cstddef>            // for std::ptrdiff_t
#include <algorithm>          // for std::swap
#include <functional>         // for std::less

#include <boost/assert.hpp>
#include <boost/checked_delete.hpp>
#include <boost/detail/workaround.hpp>

#include <phxpr/gc/detail/shared_count.hpp>

namespace phxpr
{

//
//  shared_array
//
//  shared_array extends shared_ptr to arrays.
//  The array pointed to is deleted when the last shared_array pointing to it
//  is destroyed or reset.
//

template<class T> class shared_array
{
private:

    // Borland 5.5.1 specific workarounds
    typedef boost::checked_array_deleter<T> deleter;
    typedef shared_array<T> this_type;

public:

    typedef T element_type;

    explicit shared_array(T * p = 0): px(p), pn(p, deleter())
    {
    }

    //
    // Requirements: D's copy constructor must not throw
    //
    // shared_array will release p by calling d(p)
    //

    template<class D> shared_array(T * p, D d): px(p), pn(p, d)
    {
    }

//  generated copy constructor, assignment, destructor are fine

    void reset(T * p = 0)
    {
        BOOST_ASSERT(p == 0 || p != px);
        this_type(p).swap(*this);
    }

    template <class D> void reset(T * p, D d)
    {
        this_type(p, d).swap(*this);
    }

    T & operator[] (std::ptrdiff_t i) const // never throws
    {
        BOOST_ASSERT(px != 0);
        BOOST_ASSERT(i >= 0);
        return px[i];
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

    void swap(shared_array<T> & other) // never throws
    {
        std::swap(px, other.px);
        pn.swap(other.pn);
    }

private:

    T * px;                     // contained pointer
    detail::shared_count pn;    // reference counter

};  // shared_array

template<class T> inline bool operator==(shared_array<T> const & a, shared_array<T> const & b) // never throws
{
    return a.get() == b.get();
}

template<class T> inline bool operator!=(shared_array<T> const & a, shared_array<T> const & b) // never throws
{
    return a.get() != b.get();
}

template<class T> inline bool operator<(shared_array<T> const & a, shared_array<T> const & b) // never throws
{
    return std::less<T*>()(a.get(), b.get());
}

template<class T> void swap(shared_array<T> & a, shared_array<T> & b) // never throws
{
    a.swap(b);
}

} // namespace phxpr

#endif  // #ifndef PHXPR_91A9814D_8C0D_4DF4_9B0C_45207B7CE93D


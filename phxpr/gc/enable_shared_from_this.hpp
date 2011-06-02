//  Copyright 2002, 2009 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//  http://www.boost.org/libs/smart_ptr/enable_shared_from_this.html

#ifndef PHXPR_431AC12D_4FA6_4430_BE8D_6AF0CFE33B33
#define PHXPR_431AC12D_4FA6_4430_BE8D_6AF0CFE33B33

#include <phxpr/config.hpp>

#include <boost/assert.hpp>

#include <phxpr/gc/weak_ptr.hpp>
#include <phxpr/gc/shared_ptr.hpp>

namespace phxpr
{

template<class T> class enable_shared_from_this
{
protected:

    enable_shared_from_this()
    {
    }

    enable_shared_from_this(enable_shared_from_this const &)
    {
    }

    enable_shared_from_this & operator=(enable_shared_from_this const &)
    {
        return *this;
    }

    ~enable_shared_from_this()
    {
    }

public:

    shared_ptr<T> shared_from_this()
    {
        shared_ptr<T> p( weak_this_ );
        BOOST_ASSERT( p.get() == this );
        return p;
    }

    shared_ptr<T const> shared_from_this() const
    {
        shared_ptr<T const> p( weak_this_ );
        BOOST_ASSERT( p.get() == this );
        return p;
    }

public: // actually private, but avoids compiler template friendship issues

    // Note: invoked automatically by shared_ptr; do not call
    template<class X, class Y> void _internal_accept_owner( shared_ptr<X> const * ppx, Y * py ) const
    {
        if( weak_this_.expired() )
        {
            weak_this_ = shared_ptr<T>( *ppx, py );
        }
    }

private:

    mutable weak_ptr<T> weak_this_;
};

} // namespace phxpr

#endif  // #ifndef PHXPR_431AC12D_4FA6_4430_BE8D_6AF0CFE33B33


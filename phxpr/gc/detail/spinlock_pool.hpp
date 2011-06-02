//  Copyright (c) 2008 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  spinlock_pool<0> is reserved for atomic<>, when/if it arrives
//  spinlock_pool<1> is reserved for shared_ptr reference counts
//  spinlock_pool<2> is reserved for shared_ptr atomic access

#ifndef PHXPR_5DA447FE_705A_433D_88C0_5191A7BFD507
#define PHXPR_5DA447FE_705A_433D_88C0_5191A7BFD507

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <phxpr/config.hpp>
#include <boost/version.hpp>
#if BOOST_VERSION >= 103900
#include <boost/smart_ptr/detail/spinlock.hpp>
#else
#include <boost/detail/spinlock.hpp>
#endif
#include <cstddef>

namespace phxpr {

    template <typename Tag> 
    class spinlock_pool
    {
    private:
        static boost::detail::spinlock pool_[ 41 ];

    public:

        static boost::detail::spinlock & spinlock_for( void const * pv )
        {
            std::size_t i = reinterpret_cast< std::size_t >( pv ) % 41;
            return pool_[ i ];
        }

        class scoped_lock
        {
        private:
            boost::detail::spinlock & sp_;

            scoped_lock( scoped_lock const & );
            scoped_lock & operator=( scoped_lock const & );

        public:

            explicit scoped_lock( void const * pv ): sp_( spinlock_for( pv ) )
            {
                lock();
            }

            ~scoped_lock()
            {
                unlock();
            }

            void lock()
            {
                sp_.lock();
            }

            void unlock()
            {
                sp_.unlock();
            }
        };
    };

    template <typename Tag> 
    boost::detail::spinlock spinlock_pool<Tag>::pool_[ 41 ] =
    {
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT, BOOST_DETAIL_SPINLOCK_INIT, 
        BOOST_DETAIL_SPINLOCK_INIT
    };

} // namespace phxpr 

#endif // PHXPR_5DA447FE_705A_433D_88C0_5191A7BFD507


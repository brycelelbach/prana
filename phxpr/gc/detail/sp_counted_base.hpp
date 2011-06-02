//  Copyright 2005, 2006 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef PHXPR_8C9A4F0F_6AD8_42C7_8FD2_7C0711F246C6
#define PHXPR_8C9A4F0F_6AD8_42C7_8FD2_7C0711F246C6

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp>
#include <phxpr/gc/detail/sp_has_sync.hpp>

#if defined( PHXPR_SP_DISABLE_THREADS )
# include <phxpr/gc/detail/sp_counted_base_nt.hpp>

#elif defined( PHXPR_SP_USE_SPINLOCK )
# include <phxpr/gc/detail/sp_counted_base_spin.hpp>

#elif defined( PHXPR_SP_USE_PTHREADS )
# include <phxpr/gc/detail/sp_counted_base_pt.hpp>

#elif defined( BOOST_DISABLE_THREADS ) && !defined( PHXPR_SP_ENABLE_THREADS ) && !defined( BOOST_DISABLE_WIN32 )
# include <phxpr/gc/detail/sp_counted_base_nt.hpp>

#elif defined( __GNUC__ ) && ( defined( __i386__ ) || defined( __x86_64__ ) )
# include <phxpr/gc/detail/sp_counted_base_gcc_x86.hpp>

#elif defined( __GNUC__ ) && defined( __ia64__ ) && !defined( __INTEL_COMPILER )
# include <phxpr/gc/detail/sp_counted_base_gcc_ia64.hpp>

#elif defined(__HP_aCC) && defined(__ia64)
# include <phxpr/gc/detail/sp_counted_base_acc_ia64.hpp>

#elif defined( __MWERKS__ ) && defined( __POWERPC__ )
# include <phxpr/gc/detail/sp_counted_base_cw_ppc.hpp>

#elif defined( __GNUC__ ) && ( defined( __powerpc__ ) || defined( __ppc__ ) || defined( __ppc ) )
# include <phxpr/gc/detail/sp_counted_base_gcc_ppc.hpp>

#elif defined( __GNUC__ ) && ( defined( __mips__ ) || defined( _mips ) )
# include <phxpr/gc/detail/sp_counted_base_gcc_mips.hpp>

#elif defined( PHXPR_SP_HAS_SYNC )
# include <phxpr/gc/detail/sp_counted_base_sync.hpp>

#elif defined(__GNUC__) && ( defined( __sparcv9 ) || ( defined( __sparcv8 ) && ( __GNUC__ * 100 + __GNUC_MINOR__ >= 402 ) ) )
# include <phxpr/gc/detail/sp_counted_base_gcc_sparc.hpp>

#elif defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32__ ) || defined(__CYGWIN__)
# include <phxpr/gc/detail/sp_counted_base_w32.hpp>

#elif !defined( BOOST_HAS_THREADS )
# include <phxpr/gc/detail/sp_counted_base_nt.hpp>

#else
# include <phxpr/gc/detail/sp_counted_base_spin.hpp>

#endif

#endif  // #ifndef PHXPR_8C9A4F0F_6AD8_42C7_8FD2_7C0711F246C6


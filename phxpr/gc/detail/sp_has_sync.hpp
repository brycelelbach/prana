//  Copyright (c) 2008, 2009 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  Defines the PHXPR_SP_HAS_SYNC macro if the __sync_* intrinsics
//  are available.

#ifndef PHXPR_B7D1C001_501A_4AE4_B353_857D34BF3A96
#define PHXPR_B7D1C001_501A_4AE4_B353_857D34BF3A96

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#if defined( __GNUC__ ) && ( __GNUC__ * 100 + __GNUC_MINOR__ >= 401 ) && !defined( PHXPR_SP_NO_SYNC )

#define PHXPR_SP_HAS_SYNC

#if defined( __arm__ )  || defined( __armel__ )
#undef PHXPR_SP_HAS_SYNC
#endif

#if defined( __hppa ) || defined( __hppa__ )
#undef PHXPR_SP_HAS_SYNC
#endif

#if defined( __m68k__ )
#undef PHXPR_SP_HAS_SYNC
#endif

#if defined( __sparc__ )
#undef PHXPR_SP_HAS_SYNC
#endif

#if defined( __INTEL_COMPILER ) && !defined( __ia64__ ) && ( __INTEL_COMPILER < 1110 )
#undef PHXPR_SP_HAS_SYNC
#endif

#if defined (__PATHSCALE__)
#undef PHXPR_SP_HAS_SYNC
#endif

#endif // __GNUC__ * 100 + __GNUC_MINOR__ >= 401

#endif // #ifndef PHXPR_B7D1C001_501A_4AE4_B353_857D34BF3A96


//  Copyright (c) 2007-2011 Hartmut Kaiser
//  Copyright (c)      2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BSP_A4119BC4_2FAF_4CF7_BDE7_1AC6EE239E73)
#define BSP_A4119BC4_2FAF_4CF7_BDE7_1AC6EE239E73

#include <boost/preprocessor/stringize.hpp>

///////////////////////////////////////////////////////////////////////////////
//  The version of Prana
//
//  BSP_VERSION_FULL & 0x0000FF is the sub-minor version
//  BSP_VERSION_FULL & 0x00FF00 is the minor version
//  BSP_VERSION_FULL & 0xFF0000 is the major version

#define BSP_VERSION_FULL         0x000100

#define BSP_VERSION_MAJOR        0
#define BSP_VERSION_MINOR        1
#define BSP_VERSION_PATCHLEVEL   0

#define BSP_VERSION_STR          BOOST_PP_STRINGIZE(BSP_VERSION_MAJOR) "."  \
                                 BOOST_PP_STRINGIZE(BSP_VERSION_MINOR) "."  \
                                 BOOST_PP_STRINGIZE(BSP_VERSION_PATCHLEVEL) \
  /***/

#endif // A4119BC4_2FAF_4CF7_BDE7_1AC6EE239E73


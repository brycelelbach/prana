//  Copyright (c) 2007-2011 Hartmut Kaiser
//  Copyright (c)      2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(PHXPR_1746D202_84E8_4C4B_84A6_2C20959D6D21)
#define PHXPR_1746D202_84E8_4C4B_84A6_2C20959D6D21

#include <boost/preprocessor/stringize.hpp>

///////////////////////////////////////////////////////////////////////////////
//  The version of Prana
//
//  PHXPR_VERSION_FULL & 0x0000FF is the sub-minor version
//  PHXPR_VERSION_FULL & 0x00FF00 is the minor version
//  PHXPR_VERSION_FULL & 0xFF0000 is the major version

#define PHXPR_VERSION_FULL         0x000301

#define PHXPR_VERSION_MAJOR        0
#define PHXPR_VERSION_MINOR        3
#define PHXPR_VERSION_PATCHLEVEL   1

#define PHXPR_VERSION_STR        BOOST_PP_STRINGIZE(PHXPR_VERSION_MAJOR) "."  \
                                 BOOST_PP_STRINGIZE(PHXPR_VERSION_MINOR) "."  \
                                 BOOST_PP_STRINGIZE(PHXPR_VERSION_PATCHLEVEL) \
  /***/

#endif // PHXPR_1746D202_84E8_4C4B_84A6_2C20959D6D21


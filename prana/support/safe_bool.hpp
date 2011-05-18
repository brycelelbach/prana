/*=============================================================================
    Copyright (c) 2003 Joel de Guzman
    Copyright (c) 2003-2011 Hartmut Kaiser
    Copyright (c) 2001-2009 Peter Dimov
    Copyright (c) 2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(PRANA_95C69B3E_3F3A_420A_8054_91C39BEA3DF8)
#define PRANA_95C69B3E_3F3A_420A_8054_91C39BEA3DF8

#include <boost/detail/workaround.hpp>

// REVIEW: Consider moving to sheol.

namespace prana {

#if (defined(__SUNPRO_CC) && BOOST_WORKAROUND(__SUNPRO_CC, < 0x570)) \
  || defined(__CINT__)

  // From <boost/smart_ptr/detail/operator_bool.hpp>

  template <typename Tag>
  struct safe_bool {
   private:
    bool b;

   public:
    typedef bool type; 

    safe_bool (bool b_): b(b_) { }
 
    operator type (void) const
    { return b; }
  };

#elif defined(_MANAGED)

  // From <boost/smart_ptr/detail/operator_bool.hpp>

  template <typename Tag>
  struct safe_bool {
   private:
    bool b;

    static void true_ (safe_bool***) { }
 
   public: 
    typedef void (*type)(safe_bool***);

    safe_bool (bool b_): b(b_) { }
   
    operator type (void) const
    { return b ? true_ : 0; }
  };

#else  

  template <typename Tag>
  struct safe_bool {
   private:
    bool b;

    void true_ (void) { }
 
   public: 
    typedef void (safe_bool::*type)();

    safe_bool (bool b_): b(b_) { }

    operator type (void) const
    { return b ? &safe_bool::true_ : 0; }
  };

#endif

#define BSP_OPERATOR_SAFE_BOOL(tag, expr)        \
  operator ::prana::safe_bool<tag> (void) const  \
  { return ::prana::safe_bool<tag>(expr); }      \
  /**/

} // prana

#endif // PRANA_95C69B3E_3F3A_420A_8054_91C39BEA3DF8


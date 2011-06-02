////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PHXPR_B3F0DF00_57C5_499E_A174_EB31AD267B20)
#define PHXPR_B3F0DF00_57C5_499E_A174_EB31AD267B20

#if defined(PHXPR_SP_ENABLE_DEBUG_HOOKS)
  #include <phxpr/config.hpp>

  namespace phxpr {

  void free_unreachable_objects(bool report = false) PHXPR_EXPORT;

  std::size_t find_unreachable_objects(bool report = false) PHXPR_EXPORT;

  void sp_scalar_constructor_hook(void*, std::size_t, void*) PHXPR_EXPORT;

  void sp_array_constructor_hook(void*) PHXPR_EXPORT;

  void sp_scalar_destructor_hook(void*, std::size_t, void*) PHXPR_EXPORT;

  void sp_array_destructor_hook(void*) PHXPR_EXPORT;

  } // boost
#endif

#endif // PHXPR_B3F0DF00_57C5_499E_A174_EB31AD267B20


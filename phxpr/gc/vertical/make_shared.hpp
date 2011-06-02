//  Copyright (c) 2011       Bryce Lelbach
//  Copyright (c) 2007, 2008 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//  See http://www.boost.org/libs/smart_ptr/make_shared.html
//  for documentation.
//
//  This file intentionally has no include guard

#define PHXPR_N BOOST_PP_ITERATION()

#if defined( BOOST_HAS_RVALUE_REFS )
  template< class T, BOOST_PP_ENUM_PARAMS(PHXPR_N, class A)>
  phxpr::shared_ptr< T > make_shared(BOOST_PP_ENUM_BINARY_PARAMS(PHXPR_N, A, && a))
  {
  #if defined(PHXPR_SP_ENABLE_DEBUG_HOOKS)
      return phxpr::shared_ptr< T >(new T(
          BOOST_PP_REPEAT(PHXPR_N, PHXPR_SP_FORWARD, _)
          ));
  #else
      phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );
  
      phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );
  
      void * pv = pd->address();
  
      ::new( pv ) T(
          BOOST_PP_REPEAT(PHXPR_N, PHXPR_SP_FORWARD, _)
          );
 
      pd->set_initialized();
  
      T * pt2 = static_cast< T* >( pv );
  
      phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
      return phxpr::shared_ptr< T >( pt, pt2 );
  #endif
  }
  
  template< class T, class A, BOOST_PP_ENUM_PARAMS(PHXPR_N, class A)>
  phxpr::shared_ptr< T > allocate_shared( A const & a, BOOST_PP_ENUM_BINARY_PARAMS(PHXPR_N, A, && a))
  {
      phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );
  
      phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );
  
      void * pv = pd->address();
  
      ::new( pv ) T( 
          BOOST_PP_REPEAT(PHXPR_N, PHXPR_SP_FORWARD, _)
          );
  
      pd->set_initialized();
  
      T * pt2 = static_cast< T* >( pv );
  
      phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
      return phxpr::shared_ptr< T >( pt, pt2 );
  }
#else // C++03
  template< class T, BOOST_PP_ENUM_PARAMS(PHXPR_N, class A)>
  phxpr::shared_ptr< T > make_shared(BOOST_PP_ENUM_BINARY_PARAMS(PHXPR_N, A, const & a))
  {
  #if defined(PHXPR_SP_ENABLE_DEBUG_HOOKS)
      return phxpr::shared_ptr< T >(new T(BOOST_PP_ENUM_PARAMS(PHXPR_N, a)));
  #else
      phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ) );
  
      phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );
  
      void * pv = pd->address();
  
      ::new( pv ) T(BOOST_PP_ENUM_PARAMS(PHXPR_N, a));
      pd->set_initialized();
  
      T * pt2 = static_cast< T* >( pv );
  
      phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
      return phxpr::shared_ptr< T >( pt, pt2 );
  #endif
  }
  
  template< class T, class A, BOOST_PP_ENUM_PARAMS(PHXPR_N, class A)>
  phxpr::shared_ptr< T > allocate_shared( A const & a, BOOST_PP_ENUM_BINARY_PARAMS(PHXPR_N, A, const& a))
  {
      phxpr::shared_ptr< T > pt( static_cast< T* >( 0 ), PHXPR_SP_MSD( T ), a );
  
      phxpr::detail::sp_ms_deleter< T > * pd = phxpr::get_deleter< phxpr::detail::sp_ms_deleter< T > >( pt );
  
      void * pv = pd->address();
  
      ::new( pv ) T(BOOST_PP_ENUM_PARAMS(PHXPR_N, a));
      pd->set_initialized();
  
      T * pt2 = static_cast< T* >( pv );
  
      phxpr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
      return phxpr::shared_ptr< T >( pt, pt2 );
  }
#endif

#undef PHXPR_N


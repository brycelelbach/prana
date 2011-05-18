////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !BOOST_PP_IS_ITERATING
  #if !defined(PHXPR_7EDDE724_AD86_4E01_98C4_C450676309BA)
  #define PHXPR_7EDDE724_AD86_4E01_98C4_C450676309BA
  
  #include <phxpr/config.hpp>

  #include <prana/support/safe_bool.hpp>
  
  #include <boost/swap.hpp>
  #include <boost/shared_ptr.hpp>
  #include <boost/make_shared.hpp>
  #include <boost/fusion/include/vector.hpp>
  #include <boost/fusion/include/at_c.hpp>
 
  #include <boost/preprocessor/iteration/iterate.hpp>
  #include <boost/preprocessor/repetition/repeat.hpp>
  #include <boost/preprocessor/repetition/enum_params.hpp>
  #include <boost/preprocessor/repetition/enum_binary_params.hpp>
  #include <boost/preprocessor/facilities/intercept.hpp>
  #include <boost/preprocessor/cat.hpp>
 
  // TODO: consider moving upvalueN<> into Prana, as it's pretty generic.
  
  namespace phxpr {
 
  #define BOOST_PP_ITERATION_PARAMS_1                 \
    (3, (1, PRANA_ARITY_LIMIT, <phxpr/upvalue.hpp>))  \
    /***/

  template <typename T>
  struct upvalue0 {
  // {{{
    // {{{ shared_ptr types
    typedef typename boost::shared_ptr<T>::element_type element_type;
    typedef typename boost::shared_ptr<T>::value_type value_type;
    typedef typename boost::shared_ptr<T>::pointer pointer;
    typedef typename boost::shared_ptr<T>::reference reference;
    // }}}
  
    typedef boost::fusion::vector0<> held_type;
   
    // {{{ ctors 
    upvalue0 (void): primary_() { }
  
    upvalue0 (upvalue0 const& other):
      primary_(other.primary_) { }
  
    upvalue0 (T const& primary):
      primary_(boost::make_shared<T>(primary)) { }
  
    upvalue0 (boost::shared_ptr<T> const& primary):
      primary_(primary) { }
    // }}}
  
    upvalue0& operator= (upvalue0 const& other) {
      primary_ = other.primary_;
      return *this;
    }
  
    // {{{ reset
    void reset (void) {
      using boost::fusion::at_c;
      primary_.reset();
    }
  
    void reset (upvalue0 const& other) {
      using boost::fusion::at_c;
      primary_.reset(other.primary_);
    }
  
    void reset (T const& primary) {
      using boost::fusion::at_c;
      primary_.reset(boost::make_shared<T>(primary));
    }
  
    void reset (boost::shared_ptr<T> const& primary) {
      using boost::fusion::at_c;
      primary_.reset(primary);
    }
    // }}}
  
    T& operator* (void) const
    { return *primary_; }
  
    T* operator-> (void) const
    { return primary_.get(); }
  
    T* get (void) const
    { return primary_.get(); } 
  
    held_type held (void) const
    { return held_type(); }
  
    bool unique (void) const
    { return primary_.unique(); }
  
    long use_count (void) const
    { return primary_.use_count(); }
  
    void swap (upvalue0& other)
    { boost::swap(*this, other); }
    
    operator typename prana::safe_bool<upvalue0>::type (void) const 
    { return prana::safe_bool<upvalue0>::call(primary_); }
  
   private:
    boost::shared_ptr<T> primary_;
  }; // }}}

  #include BOOST_PP_ITERATE()

  } // phxpr
 
  #endif // PHXPR_7EDDE724_AD86_4E01_98C4_C450676309BA

#else 
  #define PHXPR_N BOOST_PP_ITERATION()

  template <typename T, BOOST_PP_ENUM_PARAMS(PHXPR_N, typename H)>
  struct BOOST_PP_CAT(upvalue, PHXPR_N) {
  // {{{
    // {{{ shared_ptr types
    typedef typename boost::shared_ptr<T>::element_type element_type;
    typedef typename boost::shared_ptr<T>::value_type value_type;
    typedef typename boost::shared_ptr<T>::pointer pointer;
    typedef typename boost::shared_ptr<T>::reference reference;
    // }}}
  
    typedef boost::fusion::BOOST_PP_CAT(vector, PHXPR_N)<
      BOOST_PP_ENUM_BINARY_PARAMS
        (PHXPR_N, boost::shared_ptr<H, > BOOST_PP_INTERCEPT)
    > held_type;
  
    // {{{ ctors
    BOOST_PP_CAT(upvalue, PHXPR_N) (void): primary_(), held_() { }
  
    BOOST_PP_CAT(upvalue, PHXPR_N) (BOOST_PP_CAT(upvalue, PHXPR_N) const& other):
      primary_(other.primary_), held_(other.held_) { }
  
    BOOST_PP_CAT(upvalue, PHXPR_N) (T const& primary,
              BOOST_PP_ENUM_BINARY_PARAMS
                (PHXPR_N, boost::shared_ptr<H, > const& h)):
      primary_(boost::make_shared<T>(primary)),
      held_(BOOST_PP_ENUM_PARAMS(PHXPR_N, h)) { }
  
    BOOST_PP_CAT(upvalue, PHXPR_N) (boost::shared_ptr<T> const& primary,
              BOOST_PP_ENUM_BINARY_PARAMS
                (PHXPR_N, boost::shared_ptr<H, > const& h)):
      primary_(primary),
      held_(BOOST_PP_ENUM_PARAMS(PHXPR_N, h)) { }
    // }}}
  
    BOOST_PP_CAT(upvalue, PHXPR_N)&
    operator= (BOOST_PP_CAT(upvalue, PHXPR_N) const& other) {
      primary_ = other.primary_;
      held_ = other.held_;
      return *this;
    }
  
    // {{{ reset
    void reset (void) {
      using boost::fusion::at_c;
      primary_.reset();
      #define PHXPR_FUNC(z, n, d) at_c<n>(held_).reset();
      BOOST_PP_REPEAT(PHXPR_N, PHXPR_FUNC, _)
      #undef PHXPR_FUNC 
    }
  
    void reset (BOOST_PP_CAT(upvalue, PHXPR_N) const& other) {
      using boost::fusion::at_c;
      primary_.reset(other.primary_);
      #define PHXPR_FUNC(z, n, d) at_c<n>(held_).reset(at_c<n>(other.held_));
      BOOST_PP_REPEAT(PHXPR_N, PHXPR_FUNC, _)
      #undef PHXPR_FUNC 
    }
  
    void reset (T const& primary,
                BOOST_PP_ENUM_BINARY_PARAMS
                  (PHXPR_N, boost::shared_ptr<H, > const& h))
    {
      using boost::fusion::at_c;
      primary_.reset(boost::make_shared<T>(primary));
      #define PHXPR_FUNC(z, n, d) at_c<n>(held_).reset(BOOST_PP_CAT(h, n));
      BOOST_PP_REPEAT(PHXPR_N, PHXPR_FUNC, _)
    }
  
    void reset (boost::shared_ptr<T> const& primary,
                BOOST_PP_ENUM_BINARY_PARAMS
                  (PHXPR_N, boost::shared_ptr<H, > const& h))
    {
      using boost::fusion::at_c;
      primary_.reset(primary);
      BOOST_PP_REPEAT(PHXPR_N, PHXPR_FUNC, _)
      #undef PHXPR_FUNC
    }
    // }}}
  
    T& operator* (void) const
    { return *primary_; }
  
    T* operator-> (void) const
    { return primary_.get(); }
  
    T* get (void) const
    { return primary_.get(); } 
  
    held_type const& held (void) const
    { return held_; }
  
    bool unique (void) const
    { return primary_.unique(); }
  
    long use_count (void) const
    { return primary_.use_count(); }
  
    void swap (BOOST_PP_CAT(upvalue, PHXPR_N)& other)
    { boost::swap(*this, other); }

    operator prana::safe_bool<BOOST_PP_CAT(upvalue, PHXPR_N)> (void) const 
    { return prana::safe_bool<BOOST_PP_CAT(upvalue, PHXPR_N)>(primary_); }
  
   private:
    boost::shared_ptr<T> primary_;
    held_type held_;
  }; // }}}

  #undef PHXPR_N

#endif


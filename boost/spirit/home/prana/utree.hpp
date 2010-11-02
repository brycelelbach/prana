/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_UTREE_HPP)
#define BOOST_SPIRIT_PRANA_UTREE_HPP

#include <boost/ref.hpp>
#include <boost/cstdint.hpp>

#include <boost/spirit/home/prana/adt/sequence.hpp>
#include <boost/spirit/home/prana/adt/symbol.hpp>
#include <boost/spirit/home/prana/adt/range.hpp>

#include <boost/spirit/home/prana/kind.hpp>
#include <boost/spirit/home/prana/domain.hpp>

#include <boost/spirit/home/prana/policy/default.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename Policy = policy::default_>
class utree {
 public:
  typedef typename Policy::char_type     char_type;
  typedef typename Policy::error_handler error_handler;

  typedef utree          value_type;
  typedef utree&         reference;
  typedef utree const&   const_reference;
  typedef utree*         pointer;
  typedef utree const*   const_pointer;
  typedef std::ptrdiff_t difference_type;
  typedef std::size_t    size_type;
  
  typedef adt::sequence<utree>   sequence;
  typedef adt::symbol<char_type> symbol;

  utree (void); // default ctor

  utree (utree const&); // policy default 

  utree& operator= (utree const&);  

  kind_type kind (void) const;
  void kind (kind_type); 

 private:
  boost::uint8_t _kind;
  union {
    pointer   _pointer; 
    sequence  _sequence;
    symbol    _symbol;
  }; 
};

template<typename Policy>
utree<Policy>::utree (void) {
  // we leave a line here for breakpointing in a debugger 
}

template<typename Policy>
utree<Policy>::utree (utree const& other) {
  // FIXME: implement 
}

template<typename Policy>
utree<Policy>& utree<Policy>::operator= (utree const& other) {
  // FIXME: implement 
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_UTREE_HPP

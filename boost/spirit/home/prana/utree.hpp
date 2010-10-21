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

#include <boost/spirit/home/prana/adt/sequence.hpp>
#include <boost/spirit/home/prana/adt/symbol.hpp>
#include <boost/spirit/home/prana/adt/range.hpp>

#include <boost/spirit/home/prana/kind.hpp>
#include <boost/spirit/home/prana/domain.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename Char, typename ErrorHandler>
struct utree_policy {
  typedef Char char_type;
  typedef ErrorHandler error_handler;
};

template<typename Policy>
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
  
  typedef adt::sequence<utree>   sequence_type;
  typedef adt::symbol<char_type> symbol_type;

  utree (void);
  utree (utree const&);

  utree& operator= (utree const&);  

 private:
  union { 
    sequence_type                      sequence;
    typename sequence_type::range_type sequence_range;
    symbol_type                        symbol;
    typename symbol_type::range_type   symbol_range;
  }; 
};

template<typename Policy>
utree<Policy>::utree (void) {
  // we leave a line here to ease breakpointing the ctor in a debugger
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

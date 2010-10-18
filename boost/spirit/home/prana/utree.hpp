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

template<typename Char>
class utree {
 public:
  typedef utree          value_type;
  typedef utree&         reference;
  typedef utree const&   const_reference;
  typedef utree*         pointer;
  typedef utree const*   const_pointer;
  typedef std::ptrdiff_t difference_type;
  typedef std::size_t    size_type;
  
  typedef sequence<utree>                           sequence_type;
  typedef typename sequence_type::iterator          sequence_iterator;  
  typedef typename sequence_type::range_type        sequence_range;  
  typedef typename sequence_type::const_iterator    sequence_const_iterator;  
  typedef typename sequence_type::const_range_type  sequence_const_range;  
 
  typedef symbol<Char>                          symbol_type;
  typedef typename symbol_type::iterator        symbol_iterator;  
  typedef typename symbol_type::const_iterator  symbol_const_iterator;  

  utree (void);
  utree (utree const&);

  utree& operator= (utree const&);  

 private:
  union data_type { 
    sequence_type   seq;
    sequence_range  seq_rge;
    symbol_type     sym;
  } data;  
};

template<typename Char>
utree<Char>::utree (void) {

}

template<typename Char>
utree<Char>::utree (utree const& other) {
  data = other.data;
}

template<typename Char>
utree<Char>& utree<Char>::operator= (utree const& other) {
  data = other.data;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_UTREE_HPP

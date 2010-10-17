/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_UTREE_HPP)
#define BOOST_SPIRIT_PRANA_UTREE_HPP

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
  
  typedef node_iterator<utree> iterator;
  typedef node_iterator<utree> const_iterator;

  typedef sequence<utree> sequence_type;
  typedef symbol<Char>    symbol_type;
  typedef range<iterator> range_type;

  utree (void);
  utree (utree const&);

  utree& operator= (utree const&);  

 private:
  union data_type { 
    sequence_type seq;
    symbol_type   sym;
    range_type    rge;
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

////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010-2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(BSP_B9D84807_053E_486C_A47D_AE22F28D5A9B)
#define BSP_B9D84807_053E_486C_A47D_AE22F28D5A9B

#include <boost/assert.hpp>
#include <boost/function.hpp>
#include <boost/fusion/include/at_c.hpp>

#include <boost/spirit/home/prana/phxpr/core/actor.hpp>
#include <boost/spirit/home/prana/phxpr/core/function.hpp>
#include <boost/spirit/home/prana/phxpr/core/quote.hpp>
#include <boost/spirit/home/prana/phxpr/core/procedure.hpp>
#include <boost/spirit/home/prana/phxpr/macro.hpp>
#include <boost/spirit/home/prana/phxpr/environment.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

std::string get_symbol(utree const& s) {
  if (s.which() != utree_type::symbol_type) {
    // TODO: make this an exception
    BOOST_ASSERT(false);
  }
 
  utf8_symbol_range_type symbol = s.get<utf8_symbol_range_type>();
  return std::string(symbol.begin(), symbol.end());
}

typedef boost::function<phxpr::function(actor_list const&)> compiled_function;

struct evaluator {
  environment<compiled_function> functions;
  environment<macro> macros;

  typedef phxpr::function result_type;

  result_type operator() (utree::invalid_type) const {
    // TODO: make this an exception
    BOOST_ASSERT(false);
    return result_type();
  }

  template<class T>
  result_type operator() (T const& val) const {
    return quote(utree(val));
  }

  result_type operator() (utf8_symbol_range_type const& str) const {
    using boost::fusion::at_c;

    { // {{{ macro expansion
      fusion::vector2<environment<macro>::const_iterator, bool> macro
        = macros[std::string(str.begin(), str.end())];

      if (at_c<1>(macro)) {
        boost::shared_ptr<matcher> match
          = at_c<0>(macro)->second.match(utree::list_type());

        if (match) {
          boost::shared_ptr<utree> expansion = match->expand();
          // TODO: make this an exception
          BOOST_ASSERT(expansion);
          return utree::visit(*expansion, *this);
        }
      }
    } // }}}

    // {{{ function invocation
    fusion::vector2<environment<compiled_function>::const_iterator, bool> func
      = functions[std::string(str.begin(), str.end())];

    if (at_c<1>(func)) {
      actor_list flist;
      return (at_c<0>(func)->second)(flist);
    } // }}}
    
    // TODO: replace with exception (identifier not found error)
    BOOST_ASSERT(false);
    return result_type();
  }

  template<class Iterator>
  result_type operator() (iterator_range<Iterator> const& range) const {
    using boost::fusion::at_c;

    typename iterator_range<Iterator>::const_iterator it = range.begin();

    std::string sym = get_symbol(*it);
    ++it;

    { // {{{ macro expansion
      fusion::vector2<environment<macro>::const_iterator, bool> macro
        = macros[sym];

      if (at_c<1>(macro)) {
        utree use(iterator_range<Iterator>(it, range.end()), spirit::shallow);

        boost::shared_ptr<matcher> match = at_c<0>(macro)->second.match(use);

        if (match) {
          boost::shared_ptr<utree> expansion = match->expand();
          // TODO: make this an exception
          BOOST_ASSERT(expansion);
          return utree::visit(*expansion, *this);
        }
      }
    } // }}}

    // {{{ function invocation
    fusion::vector2<environment<compiled_function>::const_iterator, bool> func
      = functions[sym];

    if (at_c<1>(func)) {
      utree use(iterator_range<Iterator>(it, range.end()), spirit::shallow);

      actor_list flist;

      BOOST_FOREACH(utree const& e, use) {
        flist.push_back(utree::visit(e, *this));
      }

      return (at_c<0>(func)->second)(flist);
    } // }}}
    
    // TODO: replace with exception (identifier not found error)
    BOOST_ASSERT(false);
    return result_type();
  }

  result_type operator() (any_ptr const& p) const {
    // TODO: make this an exception
    BOOST_ASSERT(false);
    return result_type();
  }

  result_type operator() (function_base const& pf) const {
    // TODO: make this an exception
    BOOST_ASSERT(false);
    return result_type();
  }

  // forwarder
  result_type operator() (utree const& ut) const {
    return utree::visit(ut, *this);
  }
}; 

} // phxpr
} // prana
} // spirit
} // boost

#endif // BSP_B9D84807_053E_486C_A47D_AE22F28D5A9B


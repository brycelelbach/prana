////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2001-2011 Joel de Guzman
//  Copyright (c) 2001-2011 Hartmut Kaiser
//  Copyright (c) 2010-2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(BOOST_SPIRIT_PRANA_9F9E7744_5299_4583_AE3B_F4D353A0C7D3)
#define BOOST_SPIRIT_PRANA_9F9E7744_5299_4583_AE3B_F4D353A0C7D3

#include <boost/spirit/home/prana/phxpr/pattern.hpp>
#include <boost/spirit/home/prana/adt/dynamic_array.hpp>

namespace boost {
namespace spirit {
namespace prana {
namespace phxpr {

struct macro {
 public:
  typedef std::set<utree> literals_type;

 private:
  std::string _keyword;
  literals_type _literals;
  dynamic_array<pattern> _definitions;

 public:
  macro (std::string const& keyword_,
         literals_type const& literals_ = literals_type()):
    _keyword(keyword_), _literals(literals_), _definitions() { }

  void add (utree const& elements, utree const& body) {
    _definitions.push_back(pattern(_literals, elements, body));
  }

  boost::shared_ptr<utree> operator[] (utree const& use) const {
    BOOST_FOREACH(pattern const& p, _definitions) {
      boost::shared_ptr<utree> m = p(use);
      if (m)
        return m;
    }
    return boost::shared_ptr<utree>();
  }

  std::string keyword (void) const {
    return _keyword;
  }
};

} // phxpr 
} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_9F9E7744_5299_4583_AE3B_F4D353A0C7D3


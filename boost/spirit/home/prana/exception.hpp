/*<-============================================================================
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================->*/

#if !defined(BOOST_SPIRIT_PRANA_EXCEPTION_HPP)
#define BOOST_SPIRIT_PRANA_EXCEPTION_HPP

#include <boost/throw_exception.hpp>

#include <boost/spirit/home/karma/string.hpp>
#include <boost/spirit/home/karma/numeric.hpp>
#include <boost/spirit/home/karma/operator.hpp>
#include <boost/spirit/home/karma/generate.hpp>
#include <boost/spirit/home/karma/generate_attr.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct invalid_type_information: std::exception {
  std::string msg;

  invalid_type_information (std::size_t type) {
    std::back_insert_iterator<std::string> sink(msg);

    using karma::lit;
    karma::uint_generator<std::size_t> size_;

    karma::generate(
      sink,
      // begin grammar
      lit("(exception 'invalid-type-information ") << size_ << ")",
      // end grammar
      type
    );
  }

  virtual ~invalid_type_information (void) throw() { }

  virtual const char* what (void) const throw() { return msg.c_str(); }
};

} /*<- prana ->*/
} /*<- spirit ->*/
} /*<- boost ->*/

#endif /*<- BOOST_SPIRIT_PRANA_EXCEPTION_HPP ->*/

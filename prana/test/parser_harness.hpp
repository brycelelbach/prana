////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010-2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PRANA_1CD583C3_DD38_4AAF_B9C9_8BBF08B6E75F)
#define PRANA_1CD583C3_DD38_4AAF_B9C9_8BBF08B6E75F

#include <prana/config.hpp>

#include <sheol/lightweight_test.hpp>

#include <prana/parse/parse_tree.hpp>
#include <prana/utree/io.hpp>

namespace prana {
namespace test {

template <typename Tag>
struct parser_harness {
  typedef parser_harness const& result_type;

  result_type operator() (std::string const& in) const { 
    parse_tree<Tag> pt(in);
    SHEOL_TEST_STREQ(pt, in);
    return *this;
  }

  result_type operator() (std::string const& in, std::string const& out) const { 
    parse_tree<Tag> pt(in);
    SHEOL_TEST_STREQ(pt, out);
    return *this;
  }
  
  result_type operator() (std::string const& in, char const* out) const { 
    parse_tree<Tag> pt(in);
    SHEOL_TEST_STREQ(pt, out);
    return *this;
  }
  
  result_type operator() (std::string const& in, utree const& val) const { 
    parse_tree<Tag> pt(in);

    std::cout << "in_type=" << pt.ast().which() << ", val_type=" << val.which() << std::endl;

    SHEOL_TEST_EQ(pt.ast(), val);
    return *this;
  }
};

} // test
} // prana

#endif // PRANA_1CD583C3_DD38_4AAF_B9C9_8BBF08B6E75F


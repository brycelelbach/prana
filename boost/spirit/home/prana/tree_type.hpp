/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_TREE_TYPE_HPP)
#define BOOST_SPIRIT_PRANA_TREE_TYPE_HPP

namespace boost {
namespace spirit {
namespace prana {

// the nil type
struct nil { };

struct tree_type {
  enum info {
    nil_type,
    bool_type,
    int_type,
    double_type,
    string_type,
    string_range_type,
    symbol_type,
    binary_type,
    list_type,
    range_type,
    reference_type,
    any_type,
    function_type
  };
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_TREE_TYPE_HPP

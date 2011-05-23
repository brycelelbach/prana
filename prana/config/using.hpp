////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010-2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PRANA_E30DCD74_F4D8_4B0C_BAFE_A10BCB31044B)
#define PRANA_E30DCD74_F4D8_4B0C_BAFE_A10BCB31044B

namespace boost {
namespace spirit {

struct utree_type;
struct utree;
struct utree_list;

struct function_base;

template <typename F>
struct stored_function;

namespace qi { }
namespace karma { }

} // spirit

namespace mpl { }
namespace phoenix { }
namespace fusion { }

} // boost

namespace prana {

namespace mpl = boost::mpl;
namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;
namespace spirit = boost::spirit;
namespace qi = boost::spirit::qi;
namespace karma = boost::spirit::karma;

using boost::spirit::utree_type;
using boost::spirit::utree;
using boost::spirit::utree_list;
using boost::spirit::function_base;
using boost::spirit::stored_function;

} // prana

#endif // PRANA_E30DCD74_F4D8_4B0C_BAFE_A10BCB31044B


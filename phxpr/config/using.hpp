////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2010-2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(PHXPR_E62A44D7_937E_4D46_8B3E_D9717C77E23B)
#define PHXPR_E62A44D7_937E_4D46_8B3E_D9717C77E23B

namespace boost {
namespace spirit {

struct utree_type;
struct utree;
struct utree_list;

struct function_base;

namespace qi { }
namespace karma { }

} // spirit

namespace mpl { }
namespace phoenix { }
namespace fusion { }

} // boost

namespace phxpr {

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

} // phxpr

#endif // PHXPR_E62A44D7_937E_4D46_8B3E_D9717C77E23B


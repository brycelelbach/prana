################################################################################
#   Copyright (c) 2011 Bryce Lelbach, Steven Brandt
#
#   Distributed under the Boost Software License, Version 1.0. (See accompanying
#   file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
################################################################################

print\
  apply(lambda call:\
    apply(lambda iff:\
      call(\
        (lambda x, y:\
          x * iff(x < 2,\
                1,\
                (y, x - 1, y)),\
         6,\
         lambda x, y:\
           x * iff(x < 2,\
                 1,\
                 (y, x - 1, y))))\
    ,\
      [lambda x, a, b:\
        call(a)\
          if x\
          else call(b)])\
    ,\
      [lambda c:\
        c[0](*c[1:])\
          if (type(c) == tuple)\
          else c])


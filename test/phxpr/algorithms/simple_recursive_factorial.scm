;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   Copyright (c) 2011 Bryce Lelbach
;   Copyright (c) 2001-2011 Joel de Guzman
;   Copyright (c) 2001-2011 Hartmut Kaiser
;
;   Distributed under the Boost Software License, Version 1.0. (See accompanying
;   file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define factorial
  (lambda (n)
    (if (<= n 0) 1
      (* n (factorial (- n 1))))))

(define test-factorial
  (lambda (n)
    (display "factorial(")
    (display n)
    (display ") ==> ")
    (display (factorial n))
    (newline)))

(for-each test-factorial '(0 1 2 3 4 5 6 7 8 9 10))


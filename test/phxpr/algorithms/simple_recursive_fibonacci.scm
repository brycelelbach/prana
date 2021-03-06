;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   Copyright (c) 2011 Bryce Lelbach
;   Copyright (c) 2001-2011 Joel de Guzman
;   Copyright (c) 2001-2011 Hartmut Kaiser
;
;   Distributed under the Boost Software License, Version 1.0. (See accompanying
;   file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define fibonacci
  (lambda (n)
    (if (< n 2) n
      (+ (fibonacci (- n 1)) (fibonacci (- n 2)))))) 

(define test-fibonacci
  (lambda (n)
    (display "fibonacci(")
    (display n)
    (display ") ==> ")
    (display (fibonacci n))
    (newline)))

(for-each test-fibonacci '(0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15))


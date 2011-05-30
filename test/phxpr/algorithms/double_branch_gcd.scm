;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   Copyright (c) 2011 Bryce Lelbach
;
;   Distributed under the Boost Software License, Version 1.0. (See accompanying
;   file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define gcd
  (lambda (a b)
    (if (= a b)
      a
      (if (> a b)
        (gcd (- a b) b)
        (gcd a (- b a))))))

(define test-gcd
  (lambda (a b)
    (display "gcd(")
    (display a)
    (display ", ")
    (display b)
    (display ") ==> ")
    (display (gcd a b))
    (newline)))

(test-gcd 28 42)
(test-gcd 2232 58)
(test-gcd 100 8)



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   Copyright (c) 2011 Bryce Lelbach
;
;   Distributed under the Boost Software License, Version 1.0. (See accompanying
;   file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define ackermann 
  (lambda (m n)
    (if (= m 0)
      (+ n 1)
      (if (= n 0)
        (ackermann (- m 1) 1)
        (ackermann (- m 1) (ackermann m (- n 1)))))))

(define test-ackermann
  (lambda (m n)
    (display "ackermann(")
    (display m)
    (display ", ")
    (display n)
    (display ") ==> ")
    (display (ackermann m n))
    (newline)))

((lambda (values)
  (for-each (lambda (m)
    (for-each (lambda (n)
      (test-ackermann m n)) values)) values)) '(0 1 2 3))


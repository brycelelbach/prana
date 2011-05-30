;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   Copyright (c) 2011 Bryce Lelbach
;   Based on Kent Dybvig's fibonacci routines
;
;   Distributed under the Boost Software License, Version 1.0. (See accompanying
;   file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define addk
  (lambda (x y k)
    (if (zero? y)
      (k x)
      (addk (+ x 1) (- y 1) k))))

(define fibonacci
  (lambda (x k)
    (if (zero? x)
      (k 0)
      (if (zero? (- x 1))
        (k 1)
        (fibonacci (- x 1)
          (lambda (a)
            (fibonacci (- (- x 1) 1)
              (lambda (b)
                (addk a b k)))))))))

(define test-fibonacci
  (lambda (n)
    (display "fibonacci(")
    (display n)
    (display ") ==> ")
    (display (fibonacci n (lambda (n) n)))
    (newline)))

(for-each test-fibonacci '(0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15))


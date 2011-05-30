;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   Copyright (c) 2011 Bryce Lelbach
;
;   Distributed under the Boost Software License, Version 1.0. (See accompanying
;   file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define fibonacci
  (lambda (n)
    (define f
      (lambda (a b p q count)
        (cond ((= count 0) b)
          ((even? count)
            (f a
              b
              (+ (* p p) (* q q))
              (+ (* q q) (* 2 p q))
              (/ count 2)))
          (else
            (f (+ (* b q) (* a q) (* a p))
              (+ (* b p) (* a q))
              p
              q
              (- count 1))))))
  (f 1 0 0 1 n)))

(define test-fibonacci
  (lambda (n)
    (display "fibonacci(")
    (display n)
    (display ") ==> ")
    (display (fibonacci n))
    (newline)))

(for-each test-fibonacci '(0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15))


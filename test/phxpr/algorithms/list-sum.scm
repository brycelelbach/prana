;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   Copyright (c) 2011 Bryce Lelbach
;
;   Distributed under the Boost Software License, Version 1.0. (See accompanying
;   file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define list-sum
  (lambda (l)
    (if (null? l)
      0
      (+ (car l) (list-sum (cdr l))))))

(define test-list-sum
  (lambda (l)
    (display "list-sum('")
    (display l)
    (display ") ==> ")
    (display (list-sum l))
    (newline)))

(test-list-sum '(1 2 3 4))
(test-list-sum '(100 4 -20))


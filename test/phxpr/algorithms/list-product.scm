;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   Copyright (c) 2011 Bryce Lelbach
;
;   Distributed under the Boost Software License, Version 1.0. (See accompanying
;   file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define list-product
  (lambda (l)
    (if (null? l)
      1
      (* (car l) (list-product (cdr l))))))

(define test-list-product
  (lambda (l)
    (display "list-product('")
    (display l)
    (display ") ==> ")
    (display (list-product l))
    (newline)))

(test-list-product '(1 2 3 4))
(test-list-product '(100 4 -20))


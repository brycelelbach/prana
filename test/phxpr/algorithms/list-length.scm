;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   Copyright (c) 2011 Bryce Lelbach
;
;   Distributed under the Boost Software License, Version 1.0. (See accompanying
;   file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define list-length
  (lambda (l)
    (if (null? l)
      0
      (+ 1 (list-length (cdr l))))))

(define test-list-length
  (lambda (l)
    (display "list-length('")
    (display l)
    (display ") ==> ")
    (display (list-length l))
    (newline)))

(test-list-length '(1 2 3 4))
(test-list-length '("foo"))
(test-list-length '())


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   Copyright (c) 2011 Bryce Lelbach
;
;   Distributed under the Boost Software License, Version 1.0. (See accompanying
;   file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define list-reverse
  (lambda (l)
    (if (null? l)
      '()
      (append (list-reverse (cdr l)) (list (car l))))))

(define test-list-reverse
  (lambda (l)
    (display "list-reverse('")
    (display l)
    (display ") ==> ")
    (display (list-reverse l))
    (newline)))

(test-list-reverse '(1 2 3 4))
(test-list-reverse '("foo"))
(test-list-reverse '())


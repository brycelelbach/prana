;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   Copyright (c) 2011 Bryce Lelbach
;
;   Distributed under the Boost Software License, Version 1.0. (See accompanying
;   file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define isort
  (lambda (l)
    (letrec
      ((insert
        (lambda (x l)
          (if (null? l)
            (list x)
            (if (<= x (car l))
              (cons x l)
              (cons (car l) (insert x (cdr l))))))))
      (if (null? l)
        '()
        (insert (car l) (isort (cdr l)))))))

(define test-isort
  (lambda (l)
    (display "isort('")
    (display l)
    (display ") ==> ")
    (display (isort l))
    (newline)))

(test-isort '(5 2 84 1 57 83 13))
(test-isort '(-6 532 7 4))
(test-isort '(5 64 2 91 4 8 9 1 2))


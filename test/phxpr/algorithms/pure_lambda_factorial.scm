;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   Copyright (c) 2011 Bryce Lelbach, Steven Brandt
;
;   Distributed under the Boost Software License, Version 1.0. (See accompanying
;   file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(for-each
  (lambda (n)
    (display "factorial(")
    (display n)
    (display ") ==> ")
    (display ((lambda (n)
      ((lambda (call)
        ((lambda (iff)
          (call
            ((lambda (x y)
              (* x (iff (< x 2)
                      1
                      (list y (- x 1) y))))
             n
             (lambda (x y)
               (* x (iff (< x 2)
                      1
                      (list y (- x 1) y))))))
        ) ; iff definition
          (lambda (x a b)
            (if x
              (call a)
              (call b))))
      ) ; call definition
        (lambda (c)
          (if (list? c)
            (apply (car c) (cdr c)) 
            c)))) n))
    (newline))
  '(0 1 2 3 4 5 6 7 8 9 10))


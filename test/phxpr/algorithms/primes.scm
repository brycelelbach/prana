;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   Copyright (c) 2011 Bryce Lelbach
;
;   Distributed under the Boost Software License, Version 1.0. (See accompanying
;   file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define interval-list
  (lambda (m n)
    (if (> m n)
      '()
      (cons m (interval-list (+ 1 m) n)))))

(define sieve
  (lambda (l)
    (define remove-multiples
      (lambda (n l)
        (if (null? l)
          '()
          (if (= (modulo (car l) n) 0) ; division test
            (remove-multiples n (cdr l))
            (cons (car l)
            (remove-multiples n (cdr l)))))))
    (if (null? l)
      '()
      (cons (car l)
	    (sieve (remove-multiples (car l) (cdr l)))))))

(define primes
  (lambda (n)
    (sieve (interval-list 2 n))))

(define test-primes
  (lambda (l)
    (display "primes(")
    (display l)
    (display ") ==> ")
    (display (primes l))
    (newline)))

(test-primes 50)
(test-primes 100)


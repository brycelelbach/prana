;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   Copyright (c) 2011 Bryce Lelbach
;
;   Distributed under the Boost Software License, Version 1.0. (See accompanying
;   file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (ldiff l subl)
  (cond
    ((null? l) '())
    ((eq? (cdr l) subl) (list (car l)))
  (else (cons (car l)
    (ldiff (cdr l) subl)))))

(define last-n
  (lambda (list n)
    (list-tail list (- (length list) n))))    

(define merge-lists
  (lambda (list-1 list-2)
    (let merge-it
      ((rest-1 list-1) (rest-2 list-2) (merged '()))
        (cond
          ((null? rest-1)
            (append (reverse merged) rest-2))
          ((null? rest-2)
            (append (reverse merged) rest-1))
          ((< (car rest-1) (car rest-2))
            (merge-it (cdr rest-1) rest-2 (cons (car rest-1) merged)))
        (else
          (merge-it rest-1 (cdr rest-2) (cons (car rest-2) merged)))))))

(define (small list)
  (or (null? list) (null? (cdr list))))

(define (right-half list)
  (last-n list (ceiling (/ (length list) 2))))

(define (left-half list)
  (ldiff list (right-half list)))

(define (merge-sort list)
  (if (small list) list
    (merge-lists
      (merge-sort (left-half list))
      (merge-sort (right-half list)))))

(define test-merge-sort
  (lambda (l)
    (display "merge-sort('")
    (display l)
    (display ") ==> ")
    (display (merge-sort l))
    (newline)))

(test-merge-sort '(5 2 84 1 57 83 13))
(test-merge-sort '(-6 532 7 4))
(test-merge-sort '(5 64 2 91 4 8 9 1 2))


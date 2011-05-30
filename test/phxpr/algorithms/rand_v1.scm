; Minimal Standard Random Number Generator
; 32 bit integer version
; CACM 31 10, oct 88

(define *seed* 0)

(define (srand seed)
  (set! *seed* seed))

(define (rand)
  (let* ((hi (quotient *seed* 127773))
         (lo (modulo *seed* 127773))
         (test (- (* 16807 lo) (* 2836 hi))))
    (if (> test 0)
        (set! *seed* test)
        (set! *seed* (+ test 2147483647)))
    *seed*))

(define (test)
  (display "implementation ")
  (srand 1)
  (let loop ((n 0))
    (if (< n 10000)
        (begin
         (rand)
         (loop (1+ n)))))
  (if (= *seed* 1043618065)
      (display "looks correct")
      (begin
       (display "failed")
       (newline)
       (display "   current seed ") (display *seed*)
       (newline)
       (display "   correct seed 1043618065")))
  (newline))

(test)


; Park & Miller, CACM 31(10), Oct 1988, 32 bit integer version.
; better constants, as proposed by Park.

(define *seed* 1)

(define (srand seed)
  (set! *seed* seed)
  *seed*)

(define (rand)
  (let ((A 48271)
	(M 2147483647)
	(Q 44488)
	(R 3399))
    (let* ((hi (quotient *seed* Q))
	   (lo (modulo *seed* Q))
	   (test (- (* A lo) (* R hi))))
      (if (> test 0)
	  (set! *seed* test)
	  (set! *seed* (+ test M)))))
  *seed*)

; poker test
; seed 1
; cards 0-9 inclusive (random 10)
; five cards per hand
; 10000 hands
;
; Poker Hand     Example    Probability  Calculated
; 5 of a kind    (aaaaa)      0.0001      0
; 4 of a kind    (aaaab)      0.0045      0.0053
; Full house     (aaabb)      0.009       0.0093
; 3 of a kind    (aaabc)      0.072       0.0682
; two pairs      (aabbc)      0.108       0.1104
; Pair           (aabcd)      0.504       0.501
; Bust           (abcde)      0.3024      0.3058

(define (random n)
  (let* ((M 2147483647)
	 (slop (modulo M n)))
    (let loop ((r (rand)))
      (if (> r slop)
	  (modulo r n)	
	  (loop (rand))))))

(define (rngtest)
  (display "implementation ")
  (srand 1)
  (let loop ((n 0))
    (if (< n 10000)
        (begin
         (rand)
         (loop (1+ n)))))
  (if (= *seed* 399268537)
      (display "looks correct")
      (begin
       (display "failed")
       (newline)
       (display "   current seed ") (display *seed*)
       (newline)
       (display "   correct seed 399268537")))
  (newline))

(rngtest)


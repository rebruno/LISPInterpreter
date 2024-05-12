(define modulo %)

(define (even n)
    (if (eq? 0 (modulo n 2)) #t #f))

(define (prime n)
    (define (prime-rec n d) 
        (if (>= d n) #t
            (if (eq? 0 (modulo n d)) #f (prime-rec n (+ d 2)))))   
    (cond 
        ((> 2 n) #f)
        ((eq? n 2) #t)
        ((eq? n 3) #t)
        ((even n) #f)
        (else (prime-rec n 3))))

(define (primes-upto n)
    (define (next-prime d lst)
        (if (> d n) lst 
            (if (prime d) (next-prime (+ d 2) (cons d lst)) (next-prime (+ d 2) lst))))
    (next-prime 3 '(2)))
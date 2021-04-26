#lang racket/base

(require
   racket/extflonum
   racket/fixnum
   racket/flonum
   racket/function
   racket/list
   racket/vector
   "target-config.rkt"
   "environment.rkt")

(provide rl0.0 rl* rl/ rl+ rl- rl-neg rl-sin 
         rl-cos ->rl inexact->rl rl-sqrt 
         rl-sqr rl-expt rl-vector-ref
         list->rl-vector make-rl-vector any-number?)

(define rl0.0
  (if (equal? target-real-implementation "double")
     0.0
     0.0t0))

(define rl*
  (if (equal? target-real-implementation "double")
    fl*
    extfl*))

(define rl/
  (if (equal? target-real-implementation "double")
    fl/
    extfl/))

(define rl+
  (if (equal? target-real-implementation "double")
    fl+
    extfl+))

(define rl-
  (if (equal? target-real-implementation "double")
    fl-
    extfl-))

(define (rl-neg n)
  (if (equal? target-real-implementation "double")
    (fl- n)
    (extfl- 0.0t0 n)))

(define rl-sin
  (if (equal? target-real-implementation "double")
    flsin
    extflsin))

(define rl-cos
  (if (equal? target-real-implementation "double")
    flcos
    extflcos))

(define ->rl
  (if (equal? target-real-implementation "double")
    ->fl
    ->extfl))

(define inexact->rl
  (if (equal? target-real-implementation "double")
    identity
    real->extfl))

(define (rl-sqr x)
  (if (equal? target-real-implementation "double")
    (lambda (x) (fl* x x))
    (lambda (x) (extfl* x x))))

(define rl-sqrt
  (if (equal? target-real-implementation "double")
    flsqrt
    extflsqrt))

(define rl-expt
  (if (equal? target-real-implementation "double")
    flexpt
    extflexpt))

(define rl-vector-ref
  (if (equal? target-real-implementation "double")
    flvector-ref
    extflvector-ref))

(define make-rl-vector
  (if (equal? target-real-implementation "double")
    make-flvector
    make-extflvector))

(define (list->rl-vector value-list)
  (if (equal? target-real-implementation "double")
    (for/flvector ((x (in-list value-list)))
                  (begin
                   (atom-number x)))
    (for/extflvector ((x (in-list value-list)))
                     (begin
                      (atom-number x)))))

(define (normilize-rl rl)
  (if (and (equal? 'racket (target-lang TARGET)) 
           (equal? target-real-implementation "double"))
    #'rl
    #'(format "~a" rl)))

(define (any-number? n)
  (if (equal? target-real-implementation "double")
    (number? n)
    (or (extflonum? n) (number? n))))


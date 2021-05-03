#lang racket

(require (for-syntax racket/base
                     syntax/parse
                     racket/syntax
                     racket/string
                     racket/match
                     racket/flonum
                     racket/extflonum
                     racket/fixnum
                     "target-config.rkt"
                     "combinators.rkt"

                     (only-in "environment.rkt" make-landau-type))
         "combinators.rkt"
         "type-utils.rkt"
         "runtime-defs.rkt"
         racket/flonum
         racket/fixnum
         racket/extflonum)

(provide (all-defined-out))

(define-for-syntax (syntax->string stx)
  (to-string (syntax->datum stx)))

(define-for-syntax (atom-number stx)
  (let ((v (syntax->datum stx)))
    (match v
      ((? number?) v)
      ((? extflonum?) v)
      ((list 'quote (? number?)) (cadr v))
      ((list 'quote (? extflonum?)) (cadr v))
      (else #f))))

(define-syntax (_0.0 stx)
 (syntax-parse stx
  (_0.0 (match (target-lang TARGET)
     ('racket
       (match (target-real-implementation TARGET) ('double #'0.0) ('long-double #'0.0t0)) )
     ('ansi-c
       (match (target-real-implementation TARGET) ('double #'0.0) ('long-double #'0.0) ('double-double #'0.0))
      )))))

(define-syntax (_1.0 stx)
 (syntax-parse stx
  (_1.0 (match (target-lang TARGET)
      ('racket (match (target-real-implementation TARGET) ('double #'1.0) ('long-double #'1.0t0)) )
      ('ansi-c #'1.0)))))

(define-syntax (_2.0 stx)
 (syntax-parse stx
  (_ (match (target-lang TARGET)
     ('racket (match (target-real-implementation TARGET) ('double #'2.0) ('long-double #'2.0t0)) )
     ('ansi-c #'2.0)))))

(define-syntax (_0.5 stx)
 (syntax-parse stx
  (_ (match (target-lang TARGET)
     ('racket (match (target-real-implementation TARGET) ('double #'0.5) ('long-double #'0.5t0)) )
     ('ansi-c #'0.5)))))

;; NOTE: `?` operator in C language
(define-syntax (_if stx)
  (syntax-parse stx
                ((_if pred true-body false-body)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx (if pred true-body false-body)))
                   ('ansi-c
                    (syntax/loc stx (c-if-expr pred true-body false-body)))))))

;; NOTE: `if` statement in C language
(define-syntax (_if-stm stx)
  (syntax-parse stx
                ((_if-stm pred true-body (~optional false-body))
                 (match (target-lang TARGET)
                   ('racket
                    (if (attribute false-body)
                      (syntax/loc stx (if pred true-body false-body))
                      (syntax/loc stx (if pred true-body (void)))))
                   ('ansi-c
                    (if (attribute false-body)
                      (syntax/loc stx (c-if pred (thunk true-body) (thunk false-body)))
                      (syntax/loc stx (c-if pred (thunk true-body)))))))))

(define-syntax (_begin stx)
  (syntax-parse stx
                ((_begin body ...)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx (begin body ...)))
                   ('ansi-c
                    (syntax/loc stx (string-append body ...)))))))

(define-syntax (_rl-vector stx)
  (syntax-parse stx
                ((_ value-list)
                 (match (target-lang TARGET)
                   ('racket
                    (with-syntax ((numvec (match (target-real-implementation TARGET)
                                            ('long-double
                                            (for/extflvector ((x (in-list (syntax-e #'value-list))))
                                                             (atom-number x))
                                            )
                                            ('double
                                            (for/flvector ((x (in-list (syntax-e #'value-list))))
                                                          (begin
                                                          ;  (println x)
                                                           (atom-number x)
                                                           ))
                                            )
                                            )
                                            ))

                      (syntax/loc stx numvec)))
                  ('ansi-c
                    (quasisyntax/loc stx (c-make-array (list #,@#'value-list))))))))

(define-syntax (_define-var stx)
  (syntax-parse stx
                ((_define-var name type)
                 (match (target-lang TARGET)
                   ('racket
                    (with-syntax* ((instantiaton-datum (instantiate #'type)))
                      (quasisyntax/loc stx (define name #,#'instantiaton-datum))))

                   ('ansi-c
                    (with-syntax ((type-expd (expand-type #'type)))
                      (quasisyntax/loc stx (c-declare-var #,(syntax->string #'name) #,#'type-expd))))))
                ((_define-var name type value (~optional const?))
                 (match (target-lang TARGET)
                   ('racket
                    (quasisyntax/loc stx (define name value)))
                   ('ansi-c
                    (with-syntax* ((type-expd (expand-type #'type))
                                   (const?_ (attribute const?)))
                                  (quasisyntax/loc
                                    stx
                                    (c-declare-var #,(syntax->string #'name) #,#'type-expd #,(if (attribute const?)
                                                                                               (quote 'static)
                                                                                               (quote 'on-stack))
                                                   (to-string value) #,#'const?_))))))
))

(define-syntax (_define-var-with-func-call stx)
  (syntax-parse stx
                ((_define-var name type func-call)
                 (match (target-lang TARGET)
                   ('racket
                    (with-syntax* ((instantiaton-datum (instantiate #'type)))
                      (syntax/loc
                       stx
                       (define name func-call))))
                   ('ansi-c
                    (with-syntax* ((type-expd (expand-type #'type))
                                   (define-var
                                     (quasisyntax/loc
                                      stx
                                      (c-declare-var #,(syntax->string #'name) #,#'type-expd 'on-stack)))
                                   (func-call-stx #`(c-indent (c-line-end #,#'func-call))))
                      (datum->syntax stx `(_begin
                                           ,#'define-var
                                           ,#'func-call-stx
                                           ))))))))

(define-syntax (_set! stx)
  (syntax-parse stx
                ((_set! var value)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx (set! var value)))
                   ('ansi-c
                    (quasisyntax/loc stx (c-set #,(syntax->string #'var) (to-string value))))))))


;; NOTE treat var as a pointer in case of the C backend.
(define-syntax (_set_by_reference! stx)
  (syntax-parse stx
                ((_ var value)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx (set! var value)))
                   ('ansi-c
                    (quasisyntax/loc stx (c-set (c-dereference #,(syntax->string #'var)) (to-string value))))))))

;; NOTE: `let` like biding in Racket, redifinition in C.
;; Should not be used twise in the same scope
;; Should not be used to bind arrays
(define-syntax (_let stx)
  (syntax-parse stx
                ((_let var type value body)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx (let ((var value)) body)))
                   ('ansi-c
                    (quasisyntax/loc stx (string-append
                                     (c-declare-var #,(syntax->string #'var) type)
                                     (c-set #,(syntax->string #'var) (to-string value))
                                     body)))))))

(define-syntax (_let-int stx)
  (syntax-parse stx
                ((_let var value body)
                 (with-syntax* ((int-type (datum->syntax stx (make-landau-type 'int #f)))
                                (int-type-expd (expand-type #'int-type)))
                   (datum->syntax stx
                                  `(_let ,#'var ,#'int-type-expd ,#'value ,#'body))))))

(define-syntax (_for stx)
  (syntax-parse stx
    ((_for var-name:id start end body (~optional pragma))
         (match (target-lang TARGET)
           ('racket
            (syntax/loc stx
                        (loop start end (lambda (var-name) body))))
           ('ansi-c
            (if (attribute pragma)
              (quasisyntax/loc stx
                               (c-for #,(syntax->string #'var-name) start end (thunk body) pragma))
              (quasisyntax/loc stx
                               (c-for #,(syntax->string #'var-name) start end (thunk body)))))))))

(define-syntax (_forever stx)
  (syntax-parse stx
                ((_forever var-name:id start body)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx
                                (loop-forever start (lambda (var-name) body))))
                   ('ansi-c
            ;; TODO: get list of bindings, take one as the loop iterator, iterate other inside body
                    (quasisyntax/loc stx
                                (c-forever #,(syntax->string #'var-name) start (thunk body))))))))

(define-syntax (_int+ stx)
  (syntax-parse stx
                ((_int+ x ...)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx (fx+ x ...)))
                   ('ansi-c
                    (syntax/loc stx (c+ x ...)))))))

(define-syntax (_int- stx)
  (syntax-parse stx
                ((_int- x y)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx (fx- x y)))
                   ('ansi-c
                    (syntax/loc stx (c- x y)))))))

(define-syntax (_int* stx)
  (syntax-parse stx
                ((_ x ...)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx (fx* x ...)))
                   ('ansi-c
                    (syntax/loc stx (c* x ...)))))))

(define-syntax (_int/ stx)
  (syntax-parse stx
                ((_ x y)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx (fxquotient x y)))
                   ('ansi-c
                    (syntax/loc stx (c/ x y)))))))

(define-syntax (_int-neg stx)
  (syntax-parse stx
                ((_int-neg x)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx (fx- 0 x)))
                   ('ansi-c
                    (syntax/loc stx (c-neg x)))))))


(define-syntax (_int= stx)
  (syntax-parse stx
                ((_ x y)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx (fx= x y)))
                   ('ansi-c
                    (syntax/loc stx (c== x y)))))))

(define-syntax (_int>= stx)
  (syntax-parse stx
                ((_ x y)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx (fx>= x y)))
                   ('ansi-c
                    (syntax/loc stx (c>= x y)))))))

(define-syntax (_int> stx)
  (syntax-parse stx
                ((_ x y)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx (fx>= x y)))
                   ('ansi-c
                    (syntax/loc stx (c>= x y)))))))

(define-syntax (_int<= stx)
  (syntax-parse stx
                ((_ x y)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx (fx<= x y)))
                   ('ansi-c
                    (syntax/loc stx (c<= x y)))))))

(define-syntax (_int< stx)
  (syntax-parse stx
                ((_ x y)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx (fx< x y)))
                   ('ansi-c
                    (syntax/loc stx (c< x y)))))))

(define-syntax (_equal? stx)
  (syntax-parse stx
                ((_ x y)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx (equal? x y)))
                   ('ansi-c
                    (syntax/loc stx (c== (to-string x) (to-string y))))))))

(define-syntax (_rl* stx)
  (syntax-parse stx
                ((_ x y)
                 (match (target-lang TARGET)
                   ('racket
                    (quasisyntax/loc stx (#,(match (target-real-implementation TARGET) ('double #'fl*) ('long-double #'extfl*)) x y)))
                   ('ansi-c
                    (syntax/loc stx (cr* (to-string x) (to-string y))))))
                ((_ x rest ...)
                 (match (target-lang TARGET)
                   ('racket
                    (quasisyntax/loc
                     stx
                     (#,(match (target-real-implementation TARGET) ('double #'fl*) ('long-double #'extfl*)) x (_rl* #,@#'(rest ...)))))
                   ('ansi-c
                    (quasisyntax/loc stx (cr* (to-string x) (_rl* #,@#'(rest ...)))))))))

(define-syntax (_rldl* stx)
(syntax-parse stx
              ((_ x y)
               (match (target-lang TARGET)
                 ('racket
                  (quasisyntax/loc stx (#,(match (target-real-implementation TARGET) ('double #'fl*) ('long-double #'extfl*)) x y)))
                 ('ansi-c
                  (syntax/loc stx (crdl* (to-string x) (to-string y))))))
              ((_ x rest ...)
               (match (target-lang TARGET)
                 ('racket
                  (quasisyntax/loc
                   stx
                   (#,(match (target-real-implementation TARGET) ('double #'fl*) ('long-double #'extfl*)) x (_rldl* #,@#'(rest ...)))))
                 ('ansi-c
                  (quasisyntax/loc stx (crdl* (to-string x) (_rldl* #,@#'(rest ...)))))))))

(define-syntax (_rldr* stx)
(syntax-parse stx
              ((_ x y)
               (match (target-lang TARGET)
                 ('racket
                  (quasisyntax/loc stx (#,(match (target-real-implementation TARGET) ('double #'fl*) ('long-double #'extfl*)) x y)))
                 ('ansi-c
                  (syntax/loc stx (crdr* (to-string x) (to-string y))))))
              ((_ x rest ...)
               (match (target-lang TARGET)
                 ('racket
                  (quasisyntax/loc
                   stx
                   (#,(match (target-real-implementation TARGET) ('double #'fl*) ('long-double #'extfl*)) x (_rldr* #,@#'(rest ...)))))
                 ('ansi-c
                  (quasisyntax/loc stx (crdr* (to-string x) (_rldr* #,@#'(rest ...)))))))))

(define-syntax (_rl+ stx)
  (syntax-parse stx
                ((_ x y)
                 (match (target-lang TARGET)
                   ('racket
                    (quasisyntax/loc stx (#,(match (target-real-implementation TARGET) ('double #'fl+) ('long-double #'extfl+)) x y)))
                   ('ansi-c
                    (syntax/loc stx (cr+ (to-string x) (to-string y))))))
                ((_ x rest ...)
                 (match (target-lang TARGET)
                   ('racket
                    (quasisyntax/loc
                     stx
                     (#,(match (target-real-implementation TARGET) ('double #'fl+) ('long-double #'extfl+)) x (_rl+ #,@#'(rest ...)))))
                   ('ansi-c
                    (quasisyntax/loc stx (cr+ (to-string x) (_rl+ #,@#'(rest ...)))))))))

(define-syntax (_rldl+ stx)
  (syntax-parse stx
              ((_ x y)
               (match (target-lang TARGET)
                 ('racket
                  (quasisyntax/loc stx (#,(match (target-real-implementation TARGET) ('double #'fl+) ('long-double #'extfl+)) x y)))
                 ('ansi-c
                  (syntax/loc stx (crdl+ (to-string x) (to-string y))))))
              ((_ x rest ...)
               (match (target-lang TARGET)
                 ('racket
                  (quasisyntax/loc
                   stx
                   (#,(match (target-real-implementation TARGET) ('double #'fl+) ('long-double #'extfl+)) x (_rldl+ #,@#'(rest ...)))))
                 ('ansi-c
                  (quasisyntax/loc stx (crdl+ (to-string x) (_rldl+ #,@#'(rest ...)))))))))


(define-syntax (_rldr+ stx)
(syntax-parse stx
            ((_ x y)
             (match (target-lang TARGET)
               ('racket
                (quasisyntax/loc stx (#,(match (target-real-implementation TARGET) ('double #'fl+) ('long-double #'extfl+)) x y)))
               ('ansi-c
                (syntax/loc stx (crdr+ (to-string x) (to-string y))))))
            ((_ x rest ...)
             (match (target-lang TARGET)
               ('racket
                (quasisyntax/loc
                 stx
                 (#,(match (target-real-implementation TARGET) ('double #'fl+) ('long-double #'extfl+)) x (_rldr+ #,@#'(rest ...)))))
               ('ansi-c
                (quasisyntax/loc stx (crdr+ (to-string x) (_rldr+ #,@#'(rest ...)))))))))


(define-syntax (_rl/ stx)
  (syntax-parse stx
                ((_ x y)
                 (match (target-lang TARGET)
                   ('racket
                    (quasisyntax/loc stx (#,(match (target-real-implementation TARGET) ('double #'fl/) ('long-double #'extfl/)) x y)))
                   ('ansi-c
                    (syntax/loc stx (cr/ (to-string x) (to-string y))))))))

(define-syntax (_rldl/ stx)
(syntax-parse stx
              ((_ x y)
               (match (target-lang TARGET)
                 ('racket
                  (quasisyntax/loc stx (#,(match (target-real-implementation TARGET) ('double #'fl/) ('long-double #'extfl/)) x y)))
                 ('ansi-c
                  (syntax/loc stx (crdl/ (to-string x) (to-string y))))))))

(define-syntax (_rldr/ stx)
(syntax-parse stx
              ((_ x y)
               (match (target-lang TARGET)
                 ('racket
                  (quasisyntax/loc stx (#,(match (target-real-implementation TARGET) ('double #'fl/) ('long-double #'extfl/)) x y)))
                 ('ansi-c
                  (syntax/loc stx (crdr/ (to-string x) (to-string y))))))))

(define-syntax (_rl- stx)
  (syntax-parse stx
                ((_ x y)
                 (match (target-lang TARGET)
                   ('racket
                    (quasisyntax/loc stx (#,(match (target-real-implementation TARGET) ('double #'fl-) ('long-double #'extfl-)) x y)))
                   ('ansi-c
                    (syntax/loc stx (cr- (to-string x) (to-string y))))))))

(define-syntax (_rldl- stx)
(syntax-parse stx
              ((_ x y)
               (match (target-lang TARGET)
                 ('racket
                  (quasisyntax/loc stx (#,(match (target-real-implementation TARGET) ('double #'fl-) ('long-double #'extfl-)) x y)))
                 ('ansi-c
                  (syntax/loc stx (crdl- (to-string x) (to-string y))))))))

(define-syntax (_rldr- stx)
(syntax-parse stx
              ((_ x y)
               (match (target-lang TARGET)
                 ('racket
                  (quasisyntax/loc stx (#,(match (target-real-implementation TARGET) ('double #'fl-) ('long-double #'extfl-)) x y)))
                 ('ansi-c
                  (syntax/loc stx (crdr- (to-string x) (to-string y))))))))

(define-syntax (_rl-neg stx)
  (syntax-parse stx
                ((_ x)
                 (match (target-lang TARGET)
                   ('racket
                    (datum->syntax stx `(_rl- _0.0 ,#'x)))
                   ('ansi-c
                    (syntax/loc stx (c-neg (to-string x))))))))

(define-syntax (_or stx)
  (syntax-parse stx
                ((_ x y)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx (or x y)))
                   ('ansi-c
                    (syntax/loc stx (c-or x y)))))))

(define-syntax (_and stx)
  (syntax-parse stx
                ((_ x y)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx (and x y)))
                   ('ansi-c
                    (syntax/loc stx (c-and x y)))))))

(define-syntax (_not stx)
  (syntax-parse stx
                ((_ x)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx
                                (not x)))
                   ('ansi-c
                    (syntax/loc stx (c-not x)))))))

(define-syntax (_vector-ref stx)
  (syntax-parse stx
                ((_vector-ref vec idx)
                 (match (target-lang TARGET)
                   ('racket
                    (quasisyntax/loc
                     stx
                     (#,(match (target-real-implementation TARGET) ('double #'flvector-ref) ('long-double #'extflvector-ref)) vec idx)))
                   ('ansi-c
                    (quasisyntax/loc stx (c-array-ref #,(syntax->string #'vec) idx)))))))

(define-syntax (_vector-set! stx)
  (syntax-parse stx
                ((_vector-set! vec idx value)

                 (match (target-lang TARGET)
                   ('racket
                    (quasisyntax/loc
                     stx
                     (#,(match (target-real-implementation TARGET) ('double #'flvector-set!) ('long-double #'extflvector-set!)) vec idx value)))
                   ('ansi-c
                    (quasisyntax/loc stx (c-set-array #,(syntax->string #'vec) idx (to-string value))))))))

(define-syntax (_exact->inexact stx)
  (syntax-parse stx
                ((_exact->inexact exact)
                 (match (target-lang TARGET)
                   ('racket
                    (quasisyntax/loc stx
                                     (#,(match (target-real-implementation TARGET) ('double #'->fl) ('long-double #'->extfl)) exact)))
                   ('ansi-c
                    (syntax/loc stx (c-exact->inexact exact)))))))

(define-syntax (_int-vector-ref stx)
  (syntax-parse stx
                ((_vector-ref vec idx)
                 (match (target-lang TARGET)
                  ('racket
                   (syntax/loc stx
                               (fxvector-ref vec idx)))
                  ('ansi-c
                   (quasisyntax/loc stx
                               (c-array-ref #,(syntax->string #'vec) idx)))))))

;; NOTE: there is no var slice variable binding in C, because binding is replased by string like "int var = 1;"
(define-syntax (_var-ref stx)
  (syntax-parse stx
                ((_ var)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx var))
                   ('ansi-c
                    (if (number? (syntax->datum #'var))
                          (syntax/loc stx var)
                          (quasisyntax/loc stx
                                    #,(syntax->string #'var))))))))

(define-syntax (_expt stx)
  (syntax-parse stx
                ((_expt base exponent)
                 (match (target-lang TARGET)
                   ('racket
                    (quasisyntax/loc
                     stx
                     (#,(match (target-real-implementation TARGET) ('double flexpt) ('long-double extflexpt)) base exponent)))
                   ('ansi-c
                    (quasisyntax/loc
                     stx
                     (#,(match (target-real-implementation TARGET) ('double #'c-pow) ('long-double #'c-powl)) (to-string base) (to-string exponent))))))))

(define-syntax (_sqrt stx)
  (syntax-parse stx
                ((_ value)
                 (match (target-lang TARGET)
                   ('racket
                    (quasisyntax/loc
                     stx
                     (#,(match (target-real-implementation TARGET) ('double flexpt) ('long-double extflexpt)) value #,(datum->syntax stx '(_0.5)))))
                   ('ansi-c
                    (quasisyntax/loc
                     stx
                     (#,(match (target-real-implementation TARGET) ('double #'c-sqrt) ('long-double #'c-sqrtl) ('double-double #'c-sqrt)) (to-string value))))))))

(define-syntax (_sqr stx)
  (syntax-parse stx
                ((_ value)
                 (match (target-lang TARGET)
                   ('racket
                    (quasisyntax/loc
                     stx
                     (#,(match (target-real-implementation TARGET) ('double fl*) ('long-double extfl*)) value value)))
                   ('ansi-c
                    (quasisyntax/loc
                     stx
                     (c* (to-string value) (to-string value))))))))

(define-syntax (_cos stx)
  (syntax-parse stx
                ((_cos val)
                 (match (target-lang TARGET)
                   ('racket
                    (quasisyntax/loc
                     stx
                     (#,(match (target-real-implementation TARGET) ('double flcos) ('long-double extflcos)) val)))
                   ('ansi-c
                    (quasisyntax/loc
                     stx
                     (#,(match (target-real-implementation TARGET) ('double #'c-cos) ('long-double #'c-cosl)) (to-string val))))))))

(define-syntax (_sin stx)
  (syntax-parse stx
                ((_sin val)
                 (match (target-lang TARGET)
                   ('racket
                    (quasisyntax/loc
                     stx
                     (#,(match (target-real-implementation TARGET) ('double #'flsin) ('long-double #'extflsin)) val)))
                   ('ansi-c
                    (quasisyntax/loc
                     stx
                     (#,(match (target-real-implementation TARGET) ('double #'c-sin) ('long-double #'c-sinl)) (to-string val))))))))

(define-syntax (_nothing stx)
  (syntax-parse stx
                ((_)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx
                                (void)))
                   ('ansi-c
                    (syntax/loc stx ""))))))

(define-syntax (_empty-statement stx)
  (syntax-parse stx
                ((_)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx
                                (void)))
                   ('ansi-c
                    (syntax/loc stx ";\n"))))))

(define-syntax (_break stx)
  (syntax-parse stx
                ((_)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx
                                BREAK))
                   ('ansi-c
                    (syntax/loc stx (c-break)))))))

(define-syntax (_func-call stx)
  (syntax-parse stx
    ((_func-call func-name func-ret-symbol args-list)
     (match (target-lang TARGET)
       ('racket
        (begin
          (quasisyntax/loc stx
                           (#,#'func-name #,@#'args-list))))
       ('ansi-c
        (with-syntax* ((args-list-expanded
                         (for/list ((x (in-list (syntax-e #'args-list))))
                           ;; NOTE: if argument is a number, transform it to C notation. (To get rid of extfl numbers)
                           (if (atom-number x)
                             (to-string (atom-number x))
                             x)))
                       (args-list-stx #`(list #,@#'args-list-expanded)))
          (quasisyntax/loc
            stx
            (c-func-call #,(syntax->string #'func-name) #,(syntax->string #'func-ret-symbol) #,#'args-list-stx))))))))

(define-syntax (_pure-func-call stx)
  (syntax-parse stx
                ((_func-call func-name args-list)
                 (match (target-lang TARGET)
                   ('racket
                    (begin
                      (quasisyntax/loc stx
                                (#,#'func-name #,@#'args-list))))
                   ('ansi-c
                    (with-syntax ((args-list-stx #`(list #,@#'args-list)))
                     (quasisyntax/loc
                      stx
                      (c-pure-func-call #,(syntax->string #'func-name) #,#'args-list-stx))))))))

(define-syntax (_local stx)
  (syntax-parse stx
                ((_ body)
                 (match (target-lang TARGET)
                   ('racket
                    (syntax/loc stx
                                body))
                   ('ansi-c
                    (syntax/loc stx (c-with-brackets (thunk body))))))))

(define-syntax (_decl-real-func stx)
  (syntax-parse
    stx
    ((_decl-real-func func-name argnames argtypes body)
      (match (target-lang TARGET)
        ('racket
         (quasisyntax/loc
          stx
          (define (#,#'func-name #,@#'argnames) #,#'body)))
        ('ansi-c
         (let*
            ((c-args (string-join
                      (for/list ((argname (in-list (syntax->datum #'argnames)))
                                 (argtype (in-list (syntax->datum #'argtypes))))
                                (let ((argname-str (format "~a" argname)))
                                  (to-c-func-param argtype argname-str TARGET)))
                      ", ")))

          (with-syntax ((c-func-pragma (match (target-real-implementation TARGET) ('double "static inline double") ('long-double "static inline long double") ('double-double "static inline double*") )))
           (quasisyntax/loc
            stx
            (c-func-decl #,#'c-func-pragma #,(syntax->string #'func-name) "" #,c-args (thunk (c-return #,#'body)))))))))))

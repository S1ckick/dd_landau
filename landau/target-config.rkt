#lang racket
(require racket/base
         racket/contract
         json)

(provide target/c lang/c TARGET target-lang target-real-implementation target-c-out-path)

(struct target
    (lang
     real-implementation
     c-out-path)
    #:prefab)

(define lang/c (one-of/c 'racket 'ansi-c))
(define real-implementation/c (one-of/c 'double 'long-double 'double-double))
(define target/c (struct/c target
                           lang/c
                           real-implementation/c
                           path-for-some-system?))

(define (get-source-name)
  (let ((full-name-lst (string-split (format "~a" (find-system-path 'run-file)) ".")))
    (when (empty? full-name-lst)
      (error "Empty Landau source name"))
    (cond
      ;; FIXME: when .dau is called with drracket full-name-lst is drracket's path
      ((absolute-path? (car full-name-lst)) "landau_output")
      (else (file-name-from-path (car full-name-lst))))))

(define (path-only_ path)
  (define dir (path-only path))
  (if dir dir "./"))

(define (resolve-path relative-path)
  (let* ((source-name (get-source-name))
         (target-name (format "~a.c" source-name))
         (out-path relative-path)
         (resolved-path
          (cond
            ((absolute-path? out-path)
             (build-path out-path))
            (else
             (let*
              ((root-path (path-only_ (find-system-path 'run-file)))
               (out-dir-resolved (build-path root-path relative-path)))
              (simplify-path out-dir-resolved))))))

        (unless (directory-exists? resolved-path)
                (error (format "output_directory: directory: ~a does not exist" resolved-path)))
        (build-path resolved-path target-name)))

(define (parse-target-cfg inp-path)
  (let* ((inp (open-input-file inp-path))
         (parsed (read-json inp))
         (lang (hash-ref parsed 'target_language))
         (real-implementation (hash-ref parsed 'real-implementation))
         (path (hash-ref parsed 'output_directory)))
        (close-input-port inp)
        (unless (string? lang)
                (raise-type-error 'target_language "string" lang))
        (unless (lang/c (string->symbol lang))
                (error (format
                        "target_language should be one of: \"racket\", \"ansi-c\". Given: \"~a\""
                        (string->symbol lang))))

        (unless (string? real-implementation)
                (raise-type-error 'real-implementation "string" real-implementation))

        (unless (real-implementation/c  (string->symbol real-implementation))
                (error (format
                        "target_real-implementation should be one of: \"double\", \"double-double\", \"long-double\". Given: \"~a\""
                        (string->symbol real-implementation))))

        (unless (string? path)
                (raise-type-error 'output_directory "path" path))

        (define c-output-resolved-path (resolve-path path))

        (target (string->symbol lang)
                (string->symbol real-implementation)
                c-output-resolved-path)))

(define/contract (override-with-command-line cfg-target)
  (-> target/c
      target/c)
  (let* ((args (current-command-line-arguments))
         (config-target-lang (target-lang cfg-target))
         (config-target-real-implementation (target-real-implementation cfg-target))
         ;; FIXME: do not affect if .dau is called not from ./
         (final-target-lang (if (vector-member "-c" args)
                              'ansi-c
                              config-target-lang))
         (final-target-real-implementation (cond
                                  [(vector-member "-td" args) 'double-double]
                                  [(vector-member "-ld" args) 'long-double]
                                  [(vector-member "-d" args) 'double]
                                  [else config-target-real-implementation])))
        (target final-target-lang
                final-target-real-implementation
                (target-c-out-path cfg-target))))

(define/contract
  TARGET
  target/c
  (override-with-command-line
   (parse-target-cfg
    (collection-file-path "../config.json" "landau"))))

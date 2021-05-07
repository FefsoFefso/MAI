(defclass polynom ()
    ((polunom-symbol :initarg :var1 :reader var1)
    ;; Разреженный список термов в порядке убывания степени
     (term-list :initarg :terms :reader terms)))


(defun make-term (&key order coeff)
    (list order coeff))


(defmethod print-object ((p polynom) stream)
  (format stream "(~s) ~:{~:[~:[+~;-~]~d~[~2*~;~s~*~:;~s^~d~]~;~]~}"
          (var1 p)
          (mapcar (lambda (term)
                    (list (zerop1 (coeff term))
                          (minusp1 (coeff term))
                          (if (minusp1 (coeff term))
                              (abs (coeff term))
                              (coeff term))
                          (order term)
                          (var1 p)
                          (order term)))
                  (terms p))))


(defgeneric zerop1 (arg)
 (:method ((n number))
  (zerop n)))


(defgeneric minusp1 (arg)
 (:method ((n number))
  (minusp n)))


(defun order (term) 
    (first term))


(defun coeff (term) 
    (second term))


(defun get-coef (order coef)
    (* (max order 0) coef))


(defun get-order (order)
    (max (- order 1) 0))


(defun list-coefs (p)
    (if p (cur-coef (first p) (second p) (list-coefs (rest p))))
)


(defun cur-coef (cur next tail)
    (cond ((null next) (if (= 0 (order cur)) 
                            (cons (coeff cur) tail)
                            (cons (coeff cur) (append (get-zeros (order cur)) tail)))
          )
          ((= (order cur) (1+ (order next))) (cons (coeff cur) tail))
          (t (cons (coeff cur) (append (get-zeros (1- (- (order cur) (order next)))) tail)))
    )
)


(defun get-zeros (n)
    (make-list n :initial-element '0)
)


(defun get-coefs (p)
    (let  ((b (list-coefs (terms p)))
           (d (list (first (list-coefs (terms p)))))
           )(remove-nth 1 (reverse b))))


(defun remove-nth (n list)
  (remove-if (constantly t) list :start (1- n) :count 1))


(defun get-new-coefs (old-coefs)
    (loop for coef in old-coefs
          for index from 0 below (length old-coefs)
          collect (get-coef (+ 1 index) coef)))


(defun get-new-polynom (coefs)
    (let ((p1 (make-instance 'polynom
          :var1 'x
          :terms (loop for coef in (reverse coefs)
                       for index from 0 below (length coefs)
                             collect (make-term :order (- (length coefs) (+ 1 index)) :coeff coef)))))

    p1))


(defun der-polynom (p)
    (get-new-polynom (get-new-coefs (get-coefs p))))


(defun main ()
    (let ((p (make-instance 'polynom
          :var1 'x
          :terms (list (make-term :order 2 :coeff 5)
                       (make-term :order 1 :coeff 3.3)
                       (make-term :order 0 :coeff -7)))))
    (print (der-polynom p)))
    
    (let ((p (make-instance 'polynom
          :var1 'x
          :terms (list (make-term :order 5 :coeff 5)
                       (make-term :order 4 :coeff 3.3)
                       (make-term :order 3 :coeff -7)))))
    (print (der-polynom p)))
    
    (let ((p (make-instance 'polynom
          :var1 'x
          :terms (list (make-term :order 50 :coeff -7)))))
    (print (der-polynom p)))
    
    (let ((p (make-instance 'polynom
          :var1 'x
          :terms (list (make-term :order 6 :coeff 12)
                       (make-term :order 5 :coeff 3)
                       (make-term :order 4 :coeff 4)
                       (make-term :order 3 :coeff 5)
                       (make-term :order 2 :coeff 11.1)
                       (make-term :order 1 :coeff 19.9)
                       (make-term :order 0 :coeff 0)))))
    (print (der-polynom p)))
    
    (let ((p (make-instance 'polynom
          :var1 'x
          :terms (list (make-term :order 1 :coeff 5555.21)))))
    (print (der-polynom p))))

(main)

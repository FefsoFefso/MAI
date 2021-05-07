(defun product (func_param a b)
  (if (> a b)
      1
      (* (funcall func_param a) (product func_param (1+ a) b))))

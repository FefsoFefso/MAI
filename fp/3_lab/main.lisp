(defun get_new_matrix (array)
  (let ((matrix (make-array (list (array-dimension array 0) (array-dimension array 1))))
        (cnt (ceiling (array-dimension array 1) 2))
        (num 1))
    (loop for i from 0 below (array-dimension array 0)
        do (loop for j from 0 below (array-dimension array 1)
            do (setf (aref matrix i j) (apply 'max (get_possible_values array i j)))
                 ))
       matrix))


(defun get_possible_values (matr i_index j_index)
    (append (loop for i from 0 below (array-dimension matr 0)
      collect (aref matr i j_index))
     (loop for j from 0 below (array-dimension matr 1)
      collect (aref matr i_index j))))


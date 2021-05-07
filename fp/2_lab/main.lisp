(defun get_preorder_list (tree)
    (cond ((null tree) nil)
          ((atom (first tree))
          (cons (first tree) (get_preorder_list (rest tree))))
          (t (append (get_preorder_list(first tree)) (get_preorder_list (rest tree))))))

(defun tree-similar-p (first_tree second_tree)
    (equal (get_preorder_list first_tree) (get_preorder_list second_tree))
    )
(defun whitespace-char-p (char)
  (member char '(#\Space #\Tab #\Newline)))


(defun word-list (string)
  (loop with len = (length string)
        for left = 0 then (1+ right)
        for right = (or (position-if #'whitespace-char-p string
                                     :start left)
                        len)
        unless (= right left)
          collect (subseq string left right)
        while (< right len)))


(defun transform (bad_chars string)
    (loop for word in (word-list string)
          collect (string-right-trim bad_chars (string-left-trim bad_chars word))))


(defun make-string-from-list (list)
     (string-right-trim '(#\Space) (format nil "~{~A ~}" list)))


(defun get-words-lists (bad_chars text)
    (loop for string in text
          collect (transform bad_chars string)))


(defun text-trim (bad_chars text)
    (loop for list in (get-words-lists bad_chars text)
        collect (make-string-from-list list)))

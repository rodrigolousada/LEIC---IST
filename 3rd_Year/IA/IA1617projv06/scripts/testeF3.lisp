
;;; this is your solution file
(load "SolF3.lisp")

(defun states-to-list (stts)
  (loop for st in stts
	  collect (format nil "POS: ~a VEL: ~a ACT: ~a COST: ~a~&"
	  (state-pos st)  (state-vel st)  (state-action st)  (state-cost st))))

(defun initial-state (track)
  (make-state :pos (track-startpos track) :vel (make-vel 0 0) :action nil :cost 0 :track track))

(defvar *t1* nil)
(defvar *p1* nil)

(setf *t1* (loadtrack "20.txt"))
(setf *p1* (make-problem :initial-state (initial-state *t1*)  :fn-isGoal #'isGoalp	  :fn-nextstates #'nextStates	  :fn-h #'compute-heuristic))			  
(format t "~&Exercise 3.2 - a*~&")
 (let ((real1 (get-internal-real-time))
	   (print (format nil "~{~a~^~}" (states-to-list (a* *p1*))) ))
      (let ((real2 (get-internal-real-time)))
	  (format t "~%Computation took: ~f seconds of real time~%" (/ (- real2 real1) internal-time-units-per-second))))
		
(defvar *t2* nil)
(defvar *p2* nil)

(setf *t2* (loadtrack "20.txt"))
(setf *p2* (make-problem :initial-state (initial-state *t2*)  :fn-isGoal #'isGoalp	  :fn-nextstates #'nextStates	  :fn-h #'compute-heuristic))
			  
(format t "~&Exercise 3.2b - best-search~&")
 (let ((real1 (get-internal-real-time))
	   (print (format nil "~{~a~^~}" (states-to-list (best-search *p2*)))))
      (let ((real2 (get-internal-real-time)))
	  (format t "~%Computation took: ~f seconds of real time~%" (/ (- real2 real1) internal-time-units-per-second))))
;;;Grupo 50 - Rodrigo Lousada (81115) & Carlos Tejedor (81525)
(load "datastructures.fas")
(load "auxfuncs.fas")


;;; TAI position
(defun make-pos (c l)
  (list c l))
(defun pos-l (pos)
  (first pos))
(defun pos-c (pos)
  (second pos))

;;; TAI acceleration
(defun make-acce (c l)
  (list c l))
(defun acce-l (pos)
  (first pos))
(defun acce-c (pos)
  (second pos))

;;; TAI velocity
(defun make-vel (c l)
  (list c l))
(defun vel-l (pos)
  (first pos))
(defun vel-c (pos)
  (second pos))


;; Solution of phase 1

(defun getTrackContent (pos track)
  (nth (pos-c pos) (nth (pos-l pos) (track-env track))))

;; Pedir 0,4
(defun isObstaclep (pos track)
  "check if the position pos is an obstacle"
  (or (< (pos-l pos) 0) (< (pos-c pos) 0)
      (>= (pos-l pos) (pos-l (track-size track)))
      (>= (pos-c pos) (pos-c (track-size track)))
      (null (getTrackContent pos track))))

;; Pedir 0,4
(defun isGoalp (st) 
  "check if st is a solution of the problem"
  (let ((current-position (state-pos st))
	(track (state-track st)))
    (and (member current-position (track-endpositions track) :test #'equalp)
	 T)))

;; Pedir 1,2
(defun nextState (st act)
  "generate the nextState after state st and action act from prolem"
  (let ((new-state (make-state :action act :track (state-track st))))
    (setf (state-vel new-state)
	  (make-vel (+ (vel-l (state-vel st)) (acce-l act))
		    (+ (vel-c (state-vel st)) (acce-c act))))
    (setf (state-pos new-state)
	  (make-pos (+ (pos-l (state-pos st)) (vel-l (state-vel new-state)))
		    (+ (pos-c (state-pos st)) (vel-c (state-vel new-state)))))
    (setf (state-cost new-state)
	  (cond ((isGoalp new-state) -100)
		((isObstaclep (state-pos new-state) (state-track new-state)) 20)
		(T 1)))
    (when (= (state-cost new-state) 20)
      (setf (state-vel new-state) (make-vel 0 0))
      (setf (state-pos new-state) (make-pos (pos-l (state-pos st))
					    (pos-c (state-pos st)))))
    (values new-state)))



;; Solution of phase 2

;;; Pedir 
(defun nextStates (st)
  "generate all possible next states"
	(let ((list1))
		(setf list1 (list))
		(loop for act in (possible-actions)
			do (setf list1 (cons (nextState st act) list1))
		)
		list1
	)
)

;;; recursive_dls
(defun recursive_dls (current_st problem lim)
	"recursively return a list"
	(let ((cutoff) (aux))
		(cond ((funcall (problem-fn-isGoal problem) current_st) (return-from recursive_dls (list current_st)))
			((eq lim 0) (return-from recursive_dls ':corte))
		)
		(setf cutoff nil)
		(dolist (st (funcall (problem-fn-nextStates problem) current_st))	
			(setf aux (recursive_dls st problem (- lim 1)))
			(cond ((equal aux ':corte) (setf cutoff t))
				((not (eq aux nil)) (return-from recursive_dls (cons current_st aux))) 
			)
		)
		(cond ((eq cutoff t) (return-from recursive_dls ':corte))
			(t (return-from recursive_dls nil))
		)
	)
)
	
	
;;; limdepthfirstsearch 
(defun limdepthfirstsearch (problem lim)
  "limited depth first search
     st - initial state
     problem - problem information
     lim - depth limit"
	(return-from limdepthfirstsearch (recursive_dls (problem-initial-state problem) problem lim))
)			      

;iterlimdepthfirstsearch
(defun iterlimdepthfirstsearch (problem)
  "limited depth first search
     st - initial state
     problem - problem information
     lim - limit of depth iterations"
	(let ((current_list) (lim 0))
		(loop
			(setf current_list (limdepthfirstsearch problem lim))
			(if (not(equal current_list ':corte)) (return-from iterlimdepthfirstsearch current_list))
			(incf lim)
		)
	)
)


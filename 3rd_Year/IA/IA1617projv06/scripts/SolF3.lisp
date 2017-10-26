;;;Grupo 50 - Rodrigo Lousada (81115) & Carlos Tejedor (81525)
(load "datastructures.lisp")
(load "auxfuncs.lisp")


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
	
;; Solution of phase 3

;; Heuristic
;;BFS
(defun alreadyExists (x lista)
	(dolist (el lista)
		(if (equal el nil) (return-from alreadyExists nil))
		(if (equal (state-pos el) (state-pos x)) (return-from alreadyExists el))
	)
	nil
)

(defun compute-heuristic (st)
	(let ((openlist (list st)) (closedlist (list)) (current_st) (next_sts) (aux))
		(setf (state-other st) 0)
		(loop
			(if (equal (length openlist) 0) (return-from compute-heuristic most-positive-fixnum))
			(setf current_st (pop openlist))
			(setf aux (state-vel current_st))
			(setf (state-vel current_st) '(0 0))
			(setf next_sts (nextStates current_st))
			(setf (state-vel current_st) aux)
			(push current_st closedlist)
			(dolist (x next_sts)
				(setf (state-other x) (+ 1 (state-other current_st)))
				(if (not(alreadyExists x (append openlist closedlist))) (setf openlist (append openlist (list x))))
				(if (isGoalp x) (return-from compute-heuristic (state-other x)))
			)
		)
	)
)


;;DISTANCE
(defun calcule-distance (st)
	(let ((position (state-pos st)) (endpositions (track-endpositions (state-track st))) (solution most-positive-fixnum) (aux) (x) (y))
		(dolist (el endpositions)
				(setf x (- (first el) (first position)))
				(setf y (- (second el) (second position)))
				(setf aux (isqrt (+ (* x x) (* y y))))
				(if (< aux solution) (setf solution aux))
		)
		solution
	)
)

(defun compute-heuristic-distance (st)
	(let ((openlist (list st)) (closedlist (list)) (current_st) (next_sts) (aux))
		(setf (state-other st) 0)
		(loop
			(if (equal (length openlist) 0) (return-from compute-heuristic-distance most-positive-fixnum))
			(setf current_st (pop openlist))
			(setf aux (state-vel current_st))
			(setf (state-vel current_st) '(0 0))
			(setf next_sts (nextStates current_st))
			(setf (state-vel current_st) aux)
			(push current_st closedlist)
			(dolist (x next_sts)
				(setf (state-other x) (+ 1 (state-other current_st)))
				(if (not(alreadyExists x (append openlist closedlist))) (setf openlist (append openlist (list x))))
				(if (isGoalp x) (return-from compute-heuristic-distance (state-other x)))
			)
			(setf openlist (sort openlist #'(lambda (a b) (< (calcule-distance a) (calcule-distance b)))))
		)
	)
)

;; SOLUTION WITH Iterlimdepth

; (defun compute-heuristic (st)
	; (let* ((mystate st) (solution most-positive-fixnum) (aux) 
			; (problem (make-problem 	:initial-state mystate
									; :fn-nextStates #'nextStates
									; :fn-isGoal #'isGoalp
			; )))
		; (setf (state-vel mystate) (make-vel 0 0))
		; (setf (state-action mystate) nil)
		; (setf (state-cost mystate) 20)
		; (setf (problem-initial-state problem) mystate)
		; (setf aux (- (length (iterlimdepthfirstsearch problem)) 1))
		; (if (< aux solution) (setf solution aux))
		; (print solution)
	; )
; )

;;; A*
(defun a*_search (problem)
	(let* ((h_node (funcall (problem-fn-h problem) (problem-initial-state problem)))
			(node_start (make-node 	:state (problem-initial-state problem) 
									:g 0 
									:f h_node
									:h h_node
			)) 
			(openlist (list node_start)) 
			(node_current)
			(node_sucessor) 
			(sucessors))
		(loop
			(if (equal openlist (list)) (return-from a*_search nil))
			(setf openlist (sort openlist #'(lambda (a b) (< (node-f a) (node-f b)))))
			(setf node_current (first openlist))
			(if (funcall (problem-fn-isGoal problem) (node-state node_current)) (return-from a*_search node_current))
			(setf openlist (rest openlist))
			
			(setf sucessors (funcall (problem-fn-nextStates problem) (node-state node_current)))
			(dolist (el sucessors) 
				(setf h_node (funcall (problem-fn-h problem) el))
				(setf node_sucessor 
					(make-node 	:parent node_current 
								:state el
								:g (+ (node-g node_current) 1)
								:h h_node
								:f (+ (node-g node_current) 1 h_node)
					)
				)
				(push node_sucessor openlist)
			)
		)
	)
)

(defun a* (problem)
	(let ((solution (list)) (node_current (a*_search problem)))
		(loop while (not(eq node_current nil)) do
			(push (node-state node_current) solution)
			(setf node_current (node-parent node_current))
		)
		solution
	)
)

(defun best-search (problem)
	(setf (problem-fn-h problem) #'compute-heuristic-distance)
	(a* problem)
)
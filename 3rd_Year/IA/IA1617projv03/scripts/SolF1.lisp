
;;; These functions, and any other ones needed must be implemented

;;; Utilizar estes includes para os testes na versao local
;;; comentar antes de submeter
;(load "datastructures.lisp")
;(load "auxfuncs.lisp")

;;; Utilizar estes includes para a versao a submeter
; tirar o comentario antes de submeter
(load "datastructures.fas")
(load "auxfuncs.fas")
  
(defun isObstaclep (pos track) 
  "check if there is an obstacle at position pos of the track"
	(let ((line) (column) (access))
		(setf line (first pos))
		(setf column (second pos))
		(setf access (nth line (track-env track)))
		(setf access (nth column access))
		(eq access NIL)
	)
)
  
(defun isGoalp (st)
	"check if st is a goal state"
	(let (goal_positions)
		(setf goal_positions (track-endpositions (state-track st)))
		(loop for x in goal_positions
			do (if (equal (state-pos st) x) (return-from isGoalp t))
		)
	)
)

(defun nextState (st act)
  "generate the nextState after state st and action act"
	(let ((pos_line) (pos_column) (ac_x) (ac_y) (vel_x) (vel_y) (new_vel) (track) (new_pos) (cost 1) (next_st))
		(setf track (state-track st))
		(setf pos_line (first (state-pos st)))
		(setf pos_column (second (state-pos st)))
		(setf ac_x (first act))
		(setf ac_y (second act))
		(setf vel_x (first (state-vel st)))
		(setf vel_y (second (state-vel st)))
		(setf new_vel (list (+ vel_x ac_x) (+ vel_y ac_y)))     ; nova velocidade = velocidade + aceleracao
		(setf new_pos (list (+ pos_line (first new_vel)) (+ pos_column (second new_vel))))
		(if (isObstaclep new_pos track) 
			(progn
				(setf new_vel (list 0 0))
				(setf new_pos (state-pos st))
				(setf cost 20)
			)
		)
		(setf next_st 
			(make-STATE :POS new_pos
				:VEL new_vel
				:ACTION act
				:COST cost
				:TRACK track
			)
		)
		(if (isGoalp next_st) (setf (state-cost next_st) -100))
		next_st
   )
)
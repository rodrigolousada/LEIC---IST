
Para cada um dos tabuleiros junto foi feito o seguinte teste:
 (best-search (make-problem :initial-state (initial-state (loadtrack <track-file-name>)) :fn-isGoal #'isGoalp :fn-nextstates #'nextStates :fn-h #'compute-heuristic))

O limite de tempo foi de 5 minutos.
Foi feita a verificação da validade da solução produzida. 

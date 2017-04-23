% -----------------------------------------------------------------

%                       Resolve Manual

% -----------------------------------------------------------------

%%% mov_legal(C1, M, P, C2) - representacao de todos os movimentos legais no puzzle8
% movimentos possiveis para CIMA
mov_legal([0,B,C,D,E,F,G,H,I], c, D, [D,B,C,0,E,F,G,H,I]).
mov_legal([A,0,C,D,E,F,G,H,I], c, E, [A,E,C,D,0,F,G,H,I]).
mov_legal([A,B,0,D,E,F,G,H,I], c, F, [A,B,F,D,E,0,G,H,I]).
mov_legal([A,B,C,0,E,F,G,H,I], c, G, [A,B,C,G,E,F,0,H,I]).
mov_legal([A,B,C,D,0,F,G,H,I], c, H, [A,B,C,D,H,F,G,0,I]).
mov_legal([A,B,C,D,E,0,G,H,I], c, I, [A,B,C,D,E,I,G,H,0]).

% movimentos possiveis para BAIXO
mov_legal([A,B,C,D,E,F,G,H,0], b, F, [A,B,C,D,E,0,G,H,F]).
mov_legal([A,B,C,D,E,F,G,0,I], b, E, [A,B,C,D,0,F,G,E,I]).
mov_legal([A,B,C,D,E,F,0,H,I], b, D, [A,B,C,0,E,F,D,H,I]).
mov_legal([A,B,C,D,E,0,G,H,I], b, C, [A,B,0,D,E,C,G,H,I]).
mov_legal([A,B,C,D,0,F,G,H,I], b, B, [A,0,C,D,B,F,G,H,I]).
mov_legal([A,B,C,0,E,F,G,H,I], b, A, [0,B,C,A,E,F,G,H,I]).

% movimentos possiveis para ESQUERDA
mov_legal([0,B,C,D,E,F,G,H,I], e, B, [B,0,C,D,E,F,G,H,I]).
mov_legal([A,0,C,D,E,F,G,H,I], e, C, [A,C,0,D,E,F,G,H,I]).
mov_legal([A,B,C,0,E,F,G,H,I], e, E, [A,B,C,E,0,F,G,H,I]).
mov_legal([A,B,C,D,0,F,G,H,I], e, F, [A,B,C,D,F,0,G,H,I]).
mov_legal([A,B,C,D,E,F,0,H,I], e, H, [A,B,C,D,E,F,H,0,I]).
mov_legal([A,B,C,D,E,F,G,0,I], e, I, [A,B,C,D,E,F,G,I,0]).

% movimentos possiveis para DIREITA
mov_legal([A,0,C,D,E,F,G,H,I], d, A, [0,A,C,D,E,F,G,H,I]).
mov_legal([A,B,0,D,E,F,G,H,I], d, B, [A,0,B,D,E,F,G,H,I]).
mov_legal([A,B,C,D,0,F,G,H,I], d, D, [A,B,C,0,D,F,G,H,I]).
mov_legal([A,B,C,D,E,0,G,H,I], d, E, [A,B,C,D,0,E,G,H,I]).
mov_legal([A,B,C,D,E,F,G,0,I], d, G, [A,B,C,D,E,F,0,G,I]).
mov_legal([A,B,C,D,E,F,G,H,0], d, H, [A,B,C,D,E,F,G,0,H]).

%%% representacoes
% writte/1 - funcao identica a funcao write, no entanto, caso receba um zero escreve um espaco no seu lugar
writte(A):- A=:=0, write(' '),!.
writte(A):- write(A).

% represente/1 - recebe uma configuracao e representa-a no output
representa([A,B,C,D,E,F,G,H,I]) :- nl, write(' '), writte(A), write('  '), writte(B), write('  '), writte(C), write(' '), nl, write(' '), writte(D), write('  '), writte(E), write('  '), writte(F), write(' '), nl, write(' '), writte(G), write('  '), writte(H), write('  '), writte(I), write(' '), nl, nl.

% representa_inicial/2 - recebe duas configuracoes e desenha o objectivo, ou seja a transformacao desejada e do primeiro argumento recebido para o segundo
representa_inicial([A,B,C,D,E,F,G,H,I], [J,K,L,M,N,O,P,Q,R]) :- writeln('Transformacao desejada:'), write(' '), writte(A), write('  '), writte(B), write('  '), writte(C), write('      '), writte(J), write('  '), writte(K), write('  '), writte(L), write(' '), nl, write(' '), writte(D), write('  '), writte(E), write('  '), writte(F), write('  ->  '), writte(M), write('  '), writte(N), write('  '), writte(O), write(' '), nl, write(' '), writte(G), write('  '), writte(H), write('  '), writte(I), write('      '), writte(P), write('  '), writte(Q), write('  '), writte(R), write(' '), nl.



%%% resolve_manual/2 - predicado que recebe como primeiro argumento a configuracao incial e como segundo argumento a configuracao desejada, dando origem a solucao manual pedida no projecto
resolve_manual(C1,C2) :- representa_inicial(C1,C2), resolvendo_manual(C1,C2).
resolvendo_manual(C2,C2) :- writeln('Parabens!').
resolvendo_manual(C1,C2) :- writeln('Qual o seu movimento?'), read(M), mov_legal(C1, M, _, C3), representa(C3), resolvendo_manual(C3,C2),!.
resolvendo_manual(C1,C2) :- writeln('Movimento ilegal'), resolvendo_manual(C1,C2),!.

% -----------------------------------------------------------------

%                       Resolucao Cega

% -----------------------------------------------------------------

% escreve_solucao/1 - recebe uma lista de conjuntos (Movimento, Peça) e escreve a indicacao do movimento respectivo
escreve_solucao([(M,P) | []]) :- write('mova a peca '), write(P), traduz(M, Mp), write(Mp), write('.'), nl.
escreve_solucao([(M,P) | R]) :- write('mova a peca '), write(P), traduz(M, Mp), write(Mp), nl, escreve_solucao(R).

% traduz/2 - predicado auxiliar que permite traduzir o Movimento para se poder escrever no predicado escreve_solucao
traduz(c, ' para cima').
traduz(b, ' para baixo').
traduz(e, ' para a esquerda').
traduz(d, ' para a direita').

% inverte/2 - predicado que inverte uma lista
inverte(J,J1) :- inverte(J,J1,[]).
inverte([],J1,J1):- !.
inverte([J|R],J1,J2) :- inverte(R,J1,[J|J2]).

% resolve_cego/2 - predicado que recebe como primeiro argumento a configuracao incial e como segundo argumento a configuracao desejada, dando origem a solucao cega pedida no projecto
resolve_cego(C1,C2) :- representa_inicial(C1, C2), resolvendo_cego(C1,C2,[],[]),!.
resolvendo_cego(C2,C2,_,J) :- inverte(J,J1), escreve_solucao(J1),!.
resolvendo_cego(C1,C2,R,J) :- mov_legal(C1, M, P, C3), \+(member(C3,R)), resolvendo_cego(C3,C2,[C1|R],[(M,P)|J]).

% -----------------------------------------------------------------

%                       Resolucao Informada

% -----------------------------------------------------------------

% deslocados/3 - predicado que recebe como dois primeiros argumentos duas configuracoes de tabuleiro e possui no terceiro argumento o numero de pecas em indices diferentes, dando entao a distancia de hammig
deslocados(C1,C2,S) :- deslocados_2(C1,C2,S,0).
deslocados_2([],[],S,S_aux) :- S is S_aux.
deslocados_2([C2|R1],[C2|R2],S,S_aux) :- deslocados_2(R1,R2,S,S_aux),!.
deslocados_2([0|R1],[_|R2],S,S_aux) :- deslocados_2(R1,R2,S,S_aux),!.
deslocados_2([_|R1],[_|R2],S,S_aux) :- S_aux_2 is S_aux+1, deslocados_2(R1,R2,S,S_aux_2).

% gera_conf_possiveis/4 - recebe um no como primeiro argumento, a configuracao final pretendida como segundo, a lista de fechados como terceiro, e devolve no ultimo argumento todas os novos nos criados a partir desse
gera_conf_possiveis((C1,F,G,H,M),C2,Fechados,Novos_Nos) :-  findall([Mov,P,C3],mov_legal(C1, Mov, P, C3),L1),
							    gera_conf_possiveis_aux(F,G,H,M,C2,Fechados,L1,Novos_Nos,[]).

gera_conf_possiveis_aux(_,_,_,_,_,_,[],Novos_Nos_Aux, Novos_Nos_Aux).
gera_conf_possiveis_aux(F,G,H,M,C2,Fechados,[[Mov,P,C3]|R],Novos_Nos, Novos_Nos_Aux) :- \+(member(C3,Fechados)),
                                                                                        G1 is G+1,
											deslocados(C3,C2,H1),
											F1 is G1+H1,
											gera_conf_possiveis_aux(F,G,H,M,C2,Fechados,R,Novos_Nos,[(C3,F1,G1,H1,[(Mov,P)|M])|Novos_Nos_Aux]).
gera_conf_possiveis_aux(F,G,H,M,C2,Fechados,[[_,_,_]|R],Novos_Nos, Novos_Nos_Aux):-  gera_conf_possiveis_aux(F,G,H,M,C2,Fechados,R,Novos_Nos, Novos_Nos_Aux).

% menor/2 - recebe como primeiro argumento a lista de abertos e determina qual e devolve no segundo argumento aquele que tem o menor F
menor([First|R_Abertos], S) :- procurando_menor(R_Abertos,First,S).
procurando_menor([],S,S).
procurando_menor([(C1,F1,G1,H1,M1)|R_Abertos],(_,F2,_,_,_),S) :- F1 < F2, !, procurando_menor(R_Abertos,(C1,F1,G1,H1,M1),S).
procurando_menor([_|R_Abertos],Menor_t,S) :- procurando_menor(R_Abertos,Menor_t,S).

% resolve_info_h/2 - predicado que recebe como primeiro argumento a configuracao incial e como segundo argumento a configuracao desejada, dando origem a solucao informada segundo a heuristica de hamming pedida no projecto
resolve_info_h(C1,C2) :- representa_inicial(C1,C2), deslocados(C1,C2,H), resolvendo_info_h(C2,[(C1,H,0,H,[])],[]).

resolvendo_info_h(C2,Abertos,_) :-  menor(Abertos,(C2,_,_,H,M)),
				    H=:=0,
				    inverte(M,M1),
				    escreve_solucao(M1),!.

resolvendo_info_h(C2,Abertos,Fechados):- menor(Abertos,(Menor,F1,G1,H1,M1)),
				         gera_conf_possiveis((Menor,F1,G1,H1,M1),C2,Fechados,Novos_Nos),
					 delete(Abertos,(Menor,F1,G1,H1,M1),Abertos_R),
					 append(Novos_Nos,Abertos_R,Novos_Abertos),
					 resolvendo_info_h(C2,Novos_Abertos,[Menor|Fechados]),!.


% -----------------------------------------------------------------

%                     Transformacao Possivel

% -----------------------------------------------------------------

% superiores/3 - predicado que recebe como primeiro argumento um valor, como segundo argumento uma lista de valores, e devolve no terceiro o numero de valores anteriores ao primeiro que sao superiores
superiores(C,P,N) :- superiores(C,P,N,0).
superiores(_,[],N_aux,N_aux).
superiores(C,[P|R],N,N_aux) :- P > C,
                               N_aux_2 is N_aux+1,
			       superiores(C,R,N,N_aux_2),!.
superiores(C,[_|R],N,N_aux) :- superiores(C,R,N,N_aux),!.

% indice_possibilidade/2 - predicado que recebe uma configuracao e devolve o indice (ou caracteristica) de possibilidade
indice_possibilidade(C,I) :- indice_possibilidade(C,[],0,I).
indice_possibilidade([],_,I_temp,I_temp).
indice_possibilidade([C|R],[],I_temp,I) :- indice_possibilidade(R,C,I_temp,I),!.
indice_possibilidade([0|R],P,I_temp,I) :- indice_possibilidade(R,P,I_temp,I),!.
indice_possibilidade([C|R],P,I_temp,I) :- superiores(C,P,N),
                                          I_temp_2 is I_temp + N,
					  indice_possibilidade(R,[C|P],I_temp_2,I),!.

% transformacao_possivel/2 - predicado que recebe como primeiro argumento a configuracao incial e como segundo argumento a configuracao desejada, dando resposta a questao sobre se e possivel a transformacao do primeiro argumento para o segundo
transformacao_possivel(C1,C2) :- indice_possibilidade(C1,I1),
                                 indice_possibilidade(C2,I2),
				 (I1+I2) mod 2 =:= 0,
				 writeln('true'),!.
transformacao_possivel(_,_) :- writeln('false'),!.

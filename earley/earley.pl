% Additional predicates

addNulls(S, [X,T], Res):- (member([X,_], S) -> Res = S; append(S, [[X,T]], Res)).

addSet(S, [X,A,B,I,T], Res):- (member([X,A,B,I,_], S) -> Res = S; append(S, [[X,A,B,I,T]], Res)).

bulkAddSet(S, [], S).
bulkAddSet(S, [A|As], Res):- addSet(S, A, NewS), bulkAddSet(NewS, As, Res).

mesh(L, M, Res):- findall(T, (member(A, L), member(B, M), append(A, [B], T)), Res).

% Single steps

scan([X,A,[C|B],I,T], [X,AC,B,I,NewT]):- append(A, [C], AC), mesh(T, [[C]], NewT).

predict(Nulls, _, [X,A,[Y|B],I,Tx], _, [X,AY,B,I,NewT]):- member([Y,Ty], Nulls), append(A, [Y], AY), mesh(Tx, Ty, NewT).
predict(_, Rules, [_,_,[Y|_],_,_], K, [Y,[],RY,K,[[Y]]]):- member(RY, Rules), RY \= [].

complete([Y,_,[],_,Ty], [X,A,[Y|B],I,Tx], [X,AY,B,I,NewTx]):- append(A, [Y], AY), mesh(Tx, Ty, NewTx).

% Generate k-th set

e(_, Nulls, _, _, _, [], Curr, Next, [Curr, Next, Nulls]).

e([N,E,S,R], Nulls, [C|Cs], L, K, [[X,A,[C|B],I,T]|Xs], Curr, Next, Res):- scan([X,A,[C|B],I,T], NewNode),
	addSet(Next, NewNode, NewNext), e([N,E,S,R], Nulls, [C|Cs], L, K, Xs, Curr, NewNext, Res).

e([N,E,S,R], Nulls, [C|Cs], L, K, [[_,_,[D|_],_,_]|Xs], Curr, Next, Res):- member(D, E), D \= C,
	e([N,E,S,R], Nulls, [C|Cs], L, K, Xs, Curr, Next, Res).

e([N,E,S,R], Nulls, [], L, K, [[_,_,[C|_],_,_]|Xs], Curr, Next, Res):- member(C, E),
	e([N,E,S,R], Nulls, [], L, K, Xs, Curr, Next, Res).

e([N,E,S,R], Nulls, Cs, L, K, [[X,A,[Y|B],I,T]|_], Curr, Next, Res):- member(Y, N), member([Y|Ys], R), Node = [X,A,[Y|B],I,T],
	findall(NewNode, predict(Nulls, Ys, Node, K, NewNode), NewNodes),
	bulkAddSet(Curr, NewNodes, NewCurr), append(_, [Node|NewXs], NewCurr),
	e([N,E,S,R], Nulls, Cs, L, K, NewXs, NewCurr, Next, Res).
	
e([N,E,S,R], Nulls, Cs, L, K, [[X,A,[],K,T]|_], Curr, Next, Res):- Node = [X,A,[],K,T],
	findall(NewNode, (member(TempNode, Curr), complete(Node, TempNode, NewNode)), NewNodes),
	bulkAddSet(Curr, NewNodes, NewCurr), append(_, [Node|NewXs], NewCurr),
	addNulls(Nulls, [X,T], NewNulls), e([N,E,S,R], NewNulls, Cs, L, K, NewXs, NewCurr, Next, Res).	

e([N,E,S,R], Nulls, Cs, L, K, [[X,A,[],I,T]|_], Curr, Next, Res):- nth0(I, L, LI), Node = [X,A,[],I,T],
	findall(NewNode, (member(TempNode, LI), complete(Node, TempNode, NewNode)), NewNodes),
	bulkAddSet(Curr, NewNodes, NewCurr), append(_, [Node|NewXs], NewCurr),
	e([N,E,S,R], Nulls, Cs, L, K, NewXs, NewCurr, Next, Res).

% Main predicate
	
earley([N,E,S,R], Nulls, [], L, K, Curr, Res):- e([N,E,S,R], Nulls, [], L, K, Curr, Curr, [], [Final, _, _]),
	member([S, _, [], 0, T], Final), member(Res, T).

earley([N,E,S,R], Nulls, [C|Cs], L, K, Curr, Res):- e([N,E,S,R], Nulls, [C|Cs], L, K, Curr, Curr, [], [NewCurr, NewNext, NewNulls]), 
	M is K + 1, append(L, [NewCurr], NewL), earley([N,E,S,R], NewNulls, Cs, NewL, M, NewNext, Res).
	
earley([N,E,S,R], W, Res):- findall([X, [[X,[]]]], (member([X|Xs], R), member([], Xs)), Nulls),
	earley([N,E,S,R], Nulls, W, [], 0, [[#, [], [S], 0, [[#]]]], Res).
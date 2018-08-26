% Additional predicates

addNulls(S, [X,T], Res):- (member([X,_], S) -> Res = S; append(S, [[X,T]], Res)).

addSet(S, [X,A,B,I,T], Res):- (member([X,A,B,I,_], S) -> Res = S; append(S, [[X,A,B,I,T]], Res)).

bulkAddSet(S, [], S).
bulkAddSet(S, [A|As], Res):- addSet(S, A, NewS), bulkAddSet(NewS, As, Res).

% Single steps

scan([X,A,[C|B],I,T], [X,AC,B,I,NewT]):- append(A, [C], AC), append(T, [[C]], NewT).

predict(Nulls, _, [X,A,[Y|B],I,T], _, [X,AY,B,I,NewT]):- member([Y,Ty], Nulls), append(A, [Y], AY), append(T, [Ty], NewT).
predict(_, Rules, [_,_,[Y|_],_,_], K, [Y,[],RY,K,[Y]]):- member(RY, Rules), RY \= [].

complete([Y,_,[],_,Ty], [X,A,[Y|B],I,T], [X,AY,B,I,NewT]):- append(A, [Y], AY), append(T, [Ty], NewT).

% Generate k-th set

e(_, Nulls, _, _, _, [], Curr, Next, [Curr, Next, Nulls]).

e([N,E,S,R], Nulls, Ws, L, K, [Node|Xs], Curr, Next, Res):- Node = [_,_,[C|_],_,_], member(C,E),
	(Ws = [C|_] -> (scan(Node, NewNode), addSet(Next, NewNode, NewNext)); NewNext = Next),
	e([N,E,S,R], Nulls, Ws, L, K, Xs, Curr, NewNext, Res).

e([N,E,S,R], Nulls, Ws, L, K, [Node|_], Curr, Next, Res):- Node = [_,_,[Y|_],_,_], member([Y|Ys], R),
	findall(NewNode, predict(Nulls, Ys, Node, K, NewNode), NewNodes),
	bulkAddSet(Curr, NewNodes, NewCurr), append(_, [Node|NewXs], NewCurr),
	e([N,E,S,R], Nulls, Ws, L, K, NewXs, NewCurr, Next, Res).

e([N,E,S,R], Nulls, Ws, L, K, [Node|_], Curr, Next, Res):- Node = [X,_,[],I,T],
	(I < K -> (nth0(I, L, LI), NewNulls = Nulls); (LI = Curr, addNulls(Nulls, [X,T], NewNulls))),
	findall(NewNode, (member(TempNode, LI), complete(Node, TempNode, NewNode)), NewNodes),
	bulkAddSet(Curr, NewNodes, NewCurr), append(_, [Node|NewXs], NewCurr),
	e([N,E,S,R], NewNulls, Ws, L, K, NewXs, NewCurr, Next, Res).

% Main predicate

earley([N,E,S,R], Nulls, [], L, K, Curr, Res):- e([N,E,S,R], Nulls, [], L, K, Curr, Curr, [], [Final, _, _]),
	member([S,_,[],0,Res], Final).

earley([N,E,S,R], Nulls, [W|Ws], L, K, Curr, Res):- e([N,E,S,R], Nulls, [W|Ws], L, K, Curr, Curr, [], [NewCurr, NewNext, NewNulls]), 
	M is K + 1, append(L, [NewCurr], NewL), earley([N,E,S,R], NewNulls, Ws, NewL, M, NewNext, Res).

earley([N,E,S,R], Ws, Res):- findall([X, [X,[]]], (member([X|Xs], R), member([], Xs)), Nulls),
	earley([N,E,S,R], Nulls, Ws, [], 0, [[#,[],[S],0,[[#]]]], Res).
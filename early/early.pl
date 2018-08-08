
push(A, Y, Res):- append(A, [Y], Res).

pushSet(S, [X,A,B,I,T], Res):- (member([X,A,B,I,_], S) -> Res = S; push(S, [X,A,B,I,T], Res)).

bulkPushSet(S, [], S).
bulkPushSet(S, [A|As], Res):- pushSet(S, A, NewS), bulkPushSet(NewS, As, Res).

mesh(L, M, Res):- findall(T, (member(A, L), member(B, M), push(A, B, T)), Res).

scan([X,A,[C|B],I,T], [X,AC,B,I,NewT]):- push(A, C, AC), mesh(T, [[C]], NewT).

predict([], [X,A,[Y|B],I,T], _, [X,AY,B,I,NewT]):- push(A, Y, AY), mesh(T, [[Y,[]]], NewT).
predict([Ry|Rys], [_,_,[Y|_],_,_], K, [Y,[],[Ry|Rys],K,[[Y]]]).

complete([Y,_,[],_,Ty], [X,A,[Y|B],I,Tx], [X,AY,B,I,NewTx]):- push(A, Y, AY), mesh(Tx, Ty, NewTx).

e(_, _, _, _, [], Curr, Next, [Curr, Next]).

e([N,E,S,R], [C|Cs], L, K, [[X,A,[C|B],I,T]|Xs], Curr, Next, Res):- scan([X,A,[C|B],I,T], NewNode), pushSet(Next, NewNode, NewNext),
	e([N,E,S,R], [C|Cs], L, K, Xs, Curr, NewNext, Res).

e([N,E,S,R], [C|Cs], L, K, [[_,_,[D|_],_,_]|Xs], Curr, Next, Res):- member(D, E), D \= C,
	e([N,E,S,R], [C|Cs], L, K, Xs, Curr, Next, Res).

e([N,E,S,R], [], L, K, [[_,_,[C|_],_,_]|Xs], Curr, Next, Res):- member(C, E),
	e([N,E,S,R], [], L, K, Xs, Curr, Next, Res).

e([N,E,S,R], Cs, L, K, [[X,A,[Y|B],I,T]|_], Curr, Next, Res):- member(Y, N), member([Y|Ys], R), CurrNode = [X,A,[Y|B],I,T],
	findall(NewNode, (member(RY,Ys), predict(RY, CurrNode, K, NewNode)), NewNodes),
	bulkPushSet(Curr, NewNodes, NewCurr), append(_, [CurrNode|NewXs], NewCurr),
	e([N,E,S,R], Cs, L, K, NewXs, NewCurr, Next, Res).
	
e([N,E,S,R], Cs, L, K, [[_,_,[],K,_]|Xs], Curr, Next, Res):-
	e([N,E,S,R], Cs, L, K, Xs, Curr, Next, Res).	

e([N,E,S,R], Cs, L, K, [[X,A,[],I,T]|_], Curr, Next, Res):- nth0(I, L, LI), CurrNode = [X,A,[],I,T],
	findall(NewNode, (member(TempNode, LI), complete(CurrNode, TempNode, NewNode)), NewNodes),
	bulkPushSet(Curr, NewNodes, NewCurr), append(_, [CurrNode|NewXs], NewCurr),
	e([N,E,S,R], Cs, L, K, NewXs, NewCurr, Next, Res).

earley([N,E,S,R], [], L, K, Curr, Res):- e([N,E,S,R], [], L, K, Curr, Curr, [], [Final, _]),
	member([S, _, [], 0, T], Final), member(Res, T).

earley([N,E,S,R], [C|Cs], L, K, Curr, Res):- e([N,E,S,R], [C|Cs], L, K, Curr, Curr, [], [NewCurr, NewNext]), 
	M is K + 1, push(L, NewCurr, NewL), earley([N,E,S,R], Cs, NewL, M, NewNext, Res).
	
earley([N,E,S,R], W, Res):- earley([N,E,S,R], W, [], 0, [[#, [], [S], 0, [[#]]]], Res).

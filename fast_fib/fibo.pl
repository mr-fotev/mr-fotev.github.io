/*
Large Fibonacci numbers (N ~ 1 000 000 000)

Example usage (Time is measured in seconds):

?- fibo(10,F,Time).
F = 55,
Time = 0.0.

?- fibo(400,F,Time).
F = 176023680645013966468226945392411250770384383304492191886725992896575345044216019675,
Time = 0.0.

?- fibo(100000000,_,Time).
Time = 0.647301197052002.

?- fibo(1000000000,_,Time).
Time = 7.4540674686431885.
*/

bin(0,Bs,Bs).
bin(N,Bs,Res):-N>0, B is mod(N,2), M is div(N,2), bin(M,[B|Bs],Res).

fibo([],A,_,_,A).
fibo([X|Xs],A,B,C,Res):-AA is A^2, BB is B^2, A_ is 2*BB-3*AA-C, B_ is AA+BB,
	(X =:= 1 -> (T is A_+B_, fibo(Xs,B_,T,-2,Res)); fibo(Xs,A_,B_,2,Res)).
fibo(N,F,T):-get_time(Tstart), bin(N,[],Bs), fibo(Bs,0,1,2,F), get_time(Tend), T is Tend-Tstart, !.

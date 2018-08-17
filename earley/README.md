### Simple Earley parser in Prolog (v1.1)

* This is an inefficient program for demonstration purposes only! It's not recommended to use it for large grammars or strings, although it may run fast on some of them.

* The program expects correct input.

* Grammars representation:

    Consider G = ({S, Y}, {a,b}, S, {S -> aSb|SS|e|Y, Y -> aY|b}), where e is the empty string.  
    G can be represented by: [[nS, nY], [a,b], nS, [[nS, [a, nS, b], [nS, nS], [], [nY]], [nY, [a, nY], [b]]]]  

* Example usage:
	
```
?- G = [[nS, nY], [a,b], nS, [[nS, [a, nS, b], [nS, nS], [], [nY]], [nY, [a, nY], [b]]]], earley(G, [a,a,a,b,b,b,a,b], T).
  G = [[nS, nY], [a, b], nS, [[nS, [a, nS, b], [nS, nS], [], [nY]], [nY, [a, nY], [b]]]],
  T = [nS, [nS, [a], [nS, [a], [nS, [a], [nS, []], [b]], [b]], [b]], [nS, [a], [nS, []], [b]]] ;

  false.

  // T is a parse tree of the searched string
	
?- G = [[nS, nY], [a,b], nS, [[nS, [a, nS, b], [nS, nS], [], [nY]], [nY, [a, nY], [b]]]], earley(G, [a,b], T).
  G = [[nS, nY], [a, b], nS, [[nS, [a, nS, b], [nS, nS], [], [nY]], [nY, [a, nY], [b]]]],
  T = [nS, [a], [nS, []], [b]] ;

  G = [[nS, nY], [a, b], nS, [[nS, [a, nS, b], [nS, nS], [], [nY]], [nY, [a, nY], [b]]]],
  T = [nS, [nS, []], [nS, [a], [nS, []], [b]]] ;
	
  G = [[nS, nY], [a, b], nS, [[nS, [a, nS, b], [nS, nS], [], [nY]], [nY, [a, nY], [b]]]],
  T = [nS, [nY, [a], [nY, [b]]]] ;
	
  false.

  // Several possible trees are generated here
```
	
* The parser can not generate all parse trees

* Some test grammars  

```
g1([[nS], [a,b], nS, [[nS, [nS, nS], [a, nS, b], []]]]).
g2([[nS], [a,b], nS, [[nS, [a, nS, b], []]]]).
g3([[nS], [x, y, z, '+', '-', '*', '/', '(', ')'], nS, [[nS, [x], [y], [z], [nS, '+', nS], [nS, '-', nS], [nS, '*', nS], [nS, '/', nS], ['(', nS, ')']]]]).
even_even([[nX, nY, nS], [a, b], nS, [[nS, [nS, nS], [nX, nS], [], [nY, nS, nY]], [nX, [a, a], [b, b]], [nY, [a, b], [b, a]]]]).
```

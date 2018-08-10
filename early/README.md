### Simple Earley parser v1.0

* This program is for demonstration purposes only! It's not recommended to use it for large grammars or strings, although it may run fast on some of them.  

* The program expects correct input.

* Grammars representation:

    Consider ![](mr-fotev.github.io/early/eq1.gif)
    ![](http://latex.codecogs.com/gif.latex?G) can be represented by: [[nS, nY], [a,b], nS, [[nS, [a, nS, b], [nS, nS], [], [nY]], [nY, [a, nY], [b]]]]  

* Example usage:
	
```
?- G = [[nS, nY], [a,b], nS, [[nS, [a, nS, b], [nS, nS], [], [nY]], [nY, [a, nY], [b]]]], earley(G, [a,a,a,b,b,b,a,b], T).
  G = [[nS, nY], [a, b], nS, [[nS, [a, nS, b], [nS, nS], [], [nY]], [nY, [a, nY], [b]]]],
  T = [nS, [nS, [a], [nS, [a], [nS, [a], [nS, []], [b]], [b]], [b]], [nS, [a], [nS, []], [b]]] ;

  false.

  // T is a parse tree of the searched string.
	
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
	
* If every string in ![](http://latex.codecogs.com/gif.latex?\mathcal{L}(G)) has finitely many parse trees, you can replace the symbol '_' with 'T' on line 4 to generate all of them.  

    This also is not recommended.

* Some test grammars  

```
g1([[nS], [a,b], nS, [[nS, [nS, nS], [a, nS, b], []]]]).
g2([[nS], [a,b], nS, [[nS, [a, nS, b], []]]]).
g3([[nS], [x, y, z, '+', '-', '*', '/', '(', ')'], nS, [[nS, [x], [y], [z], [nS, '+', nS], [nS, '-', nS], [nS, '*', nS], [nS, '/', nS], ['(', nS, ')']]]]).
even_even([[nX, nY, nS], [a, b], nS, [[nS, [nS, nS], [nX, nS], [], [nY, nS, nY]], [nX, [a, a], [b, b]], [nY, [a, b], [b, a]]]])
```

// TSP_GA
function calc_route(Dist)
{
	N = Dist.length;
	M = Math.floor(N/2);
	P = 500;
	Max_Gen = 500;
	
	B = [];
	H = {};

	// fitness - sum of distances
	function theta(x)
	{
		var s = Dist[x[N - 1]][x[0]];
		for (var i = 1; i < N; i++) s += Dist[x[i - 1]][x[i]];
		return s;
	}

	// inverse mutation
	function cross(a,b)
	{
		var x = a.slice(Math.floor(M*Math.random()), M);
		x = x.concat(b.filter(function(z){return x.indexOf(z) == -1;}));
		
		if (Math.random() < 0.25)
		{
			var ia = Math.floor(N*Math.random()), ib = Math.floor(N*Math.random());
			while (ia == ib) ib = Math.floor(N*Math.random());
			
			if (ia > ib)
			{
				var t = ia;
				ia = ib;
				ib = t;
			}
			
			for (var i = ia, j = ib; i < j;)
			{
				var t = x[i];
				x[i++] = x[j];
				x[j--] = t;
			}
		}
		
		return x;
	}

	// roulette wheel-inspired selection
	function roulette(s)
	{
		var v = Math.random() * s;	

		for (var i = P - 1; i >= 0; i--)
		{
			v -= B[i].v;
			if (v <= 0) return P - 1 - i;
		}

		return P - 1;
	}
	
	// initial array
	var x = [];
	for (var i = 0; i < N; i++) x.push(i);
	
	// initial population
	for (var i = 0; i < P; i++)
	{
		for (var j = 0; j < N - 1; j++)
		{
			var u = j + Math.floor((N - j)*Math.random());
			var t = x[u];
			x[u] = x[j];
			x[j] = t;
		}
		
		H[x] = true;
		B.push({p:x.slice(), v:theta(x)});
	}
	
	B.sort(function(a,b){return a.v - b.v;});	
	
	for (var Gen = 0; Gen < Max_Gen; Gen++)
	{
		var s = 0;
		for (var i = 0; i < P; i++) s += B[i].v;
		
		for (var i = 0; i < P; i++)
		{
			var x = cross(B[roulette(s)].p,B[roulette(s)].p);
			
			if (!(x in H) || !H[x])
			{
				B.push({p:x, v:theta(x)});
				H[x] = true;
			}
		}
		
		B.sort(function(a,b){return a.v - b.v;});
		
		for (var i = P; i < B.length; i++) H[B[i].p] = false;
		
		B = B.slice(0,P);
	}
	
	return B[0];
}

onmessage = function(task)
{
	var workerResult = calc_route(task.data.dist);
	postMessage(workerResult);
}
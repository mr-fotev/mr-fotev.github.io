var B = 10000;

// long multiplication 
function mult(a,b)
{
	var n = a.length, m = b.length;
	var res = new Array(n + m);
	res.fill(0);
	
	for (var i = 0; i < n ; i++)
	{
		var c = 0;
		for (var j = 0; j < m ;j++)
		{
			res[i+j] += c + b[j]*a[i];
			c = Math.floor(res[i+j] / B);
			res[i+j] = res[i+j] % B;
		}
		res[i+m] += c;
	}
	
	if (res[n + m - 1] == 0) res.pop();
	return res;
}
// a is larger and stores the result
function sum(a, b)
{
	var n = a.length, m = b.length;
	
	if (n < m) return sum(b, a);
	
	var c = 0;
	
	for (var i = 0; i < m; i++)
	{
		a[i] = c + a[i] + b[i];
		if (a[i] > B)
		{	
			c = 1;
			a[i] -= B;
		}
		else c = 0;
	}
	
	for (var i = m; i < n; i++)
	{
		a[i] = c + a[i];
		if (a[i] > B)
		{	
			c = 1;
			a[i] -= B;
		}
		else c = 0;
	}
	
	if (c != 0) a.push(c);
	
	return a;
}

// a is larger and stores the result
function substr(a,b)
{
	var n = a.length, m = b.length;
	var c = 0;
	
	for (var i = 0; i < m; i++)
	{
		a[i] = a[i] - b[i] - c;
		if (a[i] < 0)
		{	
			c = 1;
			a[i] += B;
		}
		else c = 0;
	}
	
	for (var i = m; i < n; i++)
	{
		a[i] = a[i] - c;
		if (a[i] < 0)
		{	
			c = 1;
			a[i] += B;
		}
		else c = 0;
	}
	
	while (a[a.length - 1] == 0 && a.length > 1) a.pop();
	return a;
}

function karat(a,b)
{
	var n = Math.floor(Math.min(a.length, b.length) / 2);
	if (n < 20) return mult(a,b);
	var nulls = new Array(n);
	nulls.fill(0);
	
	var a0 = a.slice(0, n), a1 = a.slice(n, a.length), b0 = b.slice(0, n), b1 = b.slice(n, b.length);

	// delete leading nulls!!!
	while (a0[a0.length - 1] == 0) a0.pop();
	while (b0[b0.length - 1] == 0) b0.pop();
	
	var z0 = karat(a0, b0);
	var z2 = karat(a1, b1);
	var z1 = substr(substr(karat(sum(a1, a0), sum(b1,b0)), z2), z0);
	
	z2 = nulls.concat(nulls).concat(z2);
	z1 = nulls.concat(z1);
	
	return sum(sum(z2, z1), z0);
}

onmessage = function(task)
{
	var res = karat(task.data.a, task.data.b);
	postMessage({res: res});
}
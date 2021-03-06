CanonicalDot = function(n)
{	
	var a = 0, dA = 2*Math.PI/n;

	var data = [0,0,0, 0,0,1];
	for (var i=0; i<=n; i++)
	{ 
		data.push(Math.cos(a),Math.sin(a),1,0,0,1);
		a += dA;
	}
	
	var buf = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,buf);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(data), gl.STATIC_DRAW);

	this.n = n;
	this.buf = buf;
}

CanonicalDot.prototype.draw = function()
{
	gl.bindBuffer(gl.ARRAY_BUFFER,this.buf);
	
	gl.enableVertexAttribArray(aXYZ);
	gl.vertexAttribPointer(aXYZ,3,gl.FLOAT,false,6*FLOATS,0*FLOATS);
	
	gl.enableVertexAttribArray(aNormal);
	gl.vertexAttribPointer(aNormal,3,gl.FLOAT,false,6*FLOATS,3*FLOATS);

	gl.drawArrays(gl.TRIANGLE_FAN,0,this.n+2);
}

var GLOBAL_DOT;

Dot = function(center,size)
{
	this.center = center;
	this.size = size;
	this.color = [1,1,0.25];
	this.yellow = 1;
	
	// animation steps
	this.steps = 0;
}

Dot.prototype.draw = function()
{
	// animation
	if (this.steps > 0)
	{
		var t = this.yellow * 0.0375;
		this.color[1] -= t;
		this.color[2] += t;
		
		this.steps--;
		if (this.steps == 0) this.yellow = -this.yellow;
	}
	
	pushMatrix();
	gl.vertexAttrib3fv(aColor,this.color);
	translate(this.center);
	scale([this.size,this.size,1]);
	useMatrix();
	GLOBAL_DOT.draw();
	popMatrix();
}

Game = function(n)
{
	// initialize size and colors
	this.size = n;

	// initialize board with dots
	// keep in mind that B[0][0] is the bottom-left dot, B[0][1] is the dot over it and so on
	this.board = new Array(n);
	for (var i = 0; i < n; i++)
	{
		this.board[i] = new Array(n);
		for (var j = 0; j < n; j++) this.board[i][j] = new Dot([1.4*i, 1.4*j, 0], 0.6);
	}
	
	// perform some random clicks
	var u = 5*n;
	for (var i = 0; i < u; i++)
	{
		var x = Math.floor(n*Math.random());
		var y = Math.floor(n*Math.random());
		this.click(x,y,false);
	}
}

// shift the color of a single dot
Game.prototype.shift = function(i,j,s)
{
	var t = this.board[i][j].yellow;
	
	// if the smoothness_flag 's' is set then proceed with animation
	if (s)
	{
		if (this.board[i][j].steps == 0) this.board[i][j].steps = 20;
		else return;
	}
	else
	{
		this.board[i][j].yellow = -t;
		this.board[i][j].color = t == 1 ? [1,0.25,1]:[1,1,0.25];
	}
}

// click a dot
Game.prototype.click = function(i,j,s)
{
	this.shift(i,j,s);
	if (i + 1 < this.size) this.shift(i + 1, j, s);
	if (i > 0) this.shift(i - 1, j, s);
	if (j + 1 < this.size) this.shift(i, j + 1, s);
	if (j > 0) this.shift(i, j - 1, s);
}

// draw the board
Game.prototype.draw = function()
{
	for (var i = 0; i < this.size; i++) for (var j = 0; j < this.size; j++) this.board[i][j].draw();
}

// check for win
Game.prototype.win = function()
{
	var t = this.board[0][0].yellow;
	for (var i = 0; i < this.size; i++) for (var j = 0; j < this.size; j++) if (this.board[i][j].yellow != t) return false;
	return true;
}

// BrutForce solving module in O(2^n * n^2)... can be significantly improved...
Game.prototype.solve = function()
{
	var n = this.size;

	var B = new Array(n), T = new Array(n);
	for (var i = 0; i < n; i++)
	{
		B[i] = new Array(n);
		T[i] = new Array(n);
		for (var j = 0; j < n; j++) B[i][j] = this.board[j][n-1-i].yellow == 1 ? 1:0;
	}

	function cl(i,j)
	{
		T[i][j] ^= 1;
		if (i + 1 < n) T[i+1][j] ^= 1;
		if (i > 0) T[i-1][j] ^= 1;
		if (j + 1 < n) T[i][j+1] ^= 1;
		if (j > 0) T[i][j-1] ^= 1;
	}
	
	var lm = Math.pow(2, n), u = 0, k = 0;
	for (k = 0; k < lm && u < n; k++)
	{
		for (var i = 0; i < n; i++) for (var j = 0; j < n; j++) T[i][j] = B[i][j];
		for (var i = k, j = 0; i != 0; i >>= 1, j++) if (i & 1 == 1) cl(0,j);
		for (var i = 1; i < n; i++) for (var j = 0; j < n; j++) if (T[i-1][j] == 0) cl(i,j);
		for (u = 0; u < n && T[n - 1][u] == 1; u++);
	}
	
	var t = [];
	for (var i = k - 1, j = 1; i != 0; i >>= 1, j++) if (i & 1 == 1) t.push(j);
	
	var af = 'tarting from row 2 click below every purple dot!'
	return (t.length == 0 ? 'S':('Click ' + t.join(', ') + ' on row 1 and then s')) + af;
}

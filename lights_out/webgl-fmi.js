// Библиотека WebGL-FMI v0.22.11
//
// Работа с контексти и шейдъри
//		getContext(id)
//		getShader(id,type)
//		getProgram(idv,idf)
//		getVariables()
//
// Математически и помощни функции и константи
//		random(a,b)
//		radians(degrees)
//		sin(a)
//		cos(a)
//		PI
//		custom(obj,props)
//
// Работа с текстури
//		loadTexture(url,post)
//		texIdentity()
//		texTranslate(m,v)
//		texScale(m,v)
//		texRotate(m,a)
//
// Работа с вектори и матрици
//		unitVector(x)
//		vectorProduct(x,y)
//		scalarProduct(x,y)
//		vectorPoints(x,y)
//		multiplyMatrix(a,b)
//		calculateNormalMatrix(a)
//
// Трансформации с матрици
//		unitMatrix();
//		translateMatrix(v)
//		scaleMatrix(v)
//		xRotateMatrix(a)
//		yRotateMatrix(a)
//		zRotateMatrix(a)
//		rotateMatrix(a,v)
//
// Трансформации със съставни матрици
//		identity()
//		translate(v)
//		scale(v)
//		xRotate(a)
//		yRotate(a)
//		zRotate(a)
//		useMatrix()
//		pushMatrix()
//		popMatrix()
//		cloneMatrix()
//
// Проектиране с матрици
//		viewMatrix (eye, focus, up)
//		orthoMatrix(width, height, near, far)
//		perspMatrix(angle, aspect, near, far)
//		lookAt(eye, focus, up)
//		perspective(angle, aspect, near, far)
//
// Графични обекти (класове)
//		Cube(center,size) <- textured
//		Cuboid(center,size) <- textured
//		Pyramid(center,size,height,n)
//		Cone(center,size,height)
//		Conoid(center,size,ratio)
//		Prism(center,size,height,n)
//		Cylinder(center,size,height) <- textured
//		Sphere(center,size) <- textured
//		Spheroid(center,size)
//		Icosahedron(center,size)
//		GeodesicSphere(center,size)
//		RotationalSolid(center,size,f)
//		Torus(center,size,R,r) <- textured
//
// Константи
//		CONE_SIDES = 32;
//		CYLINDER_SIDES = 32;
//		SPHERE_SIDES = 32;
//		GEODESIC_SIDES = 3;
//		TORUS_MAJOR_SIDES = 50;
//		TORUS_MINOR_SIDES = 25;



var gl;				// глобален WebGL контекст
var glprog;			// глобална GLSL програма
var glmat;			// глобална матрица на модела
var glmatnew;		// true, ако матрицата е променена, но не е подадена на шейдъра
var glvmat;			// глобална матрица на гледната точка
var glstack = [];	// стек от матрици на модела


// брой байтове в един WebGL FLOAT (трябва да са 4 байта)
var FLOATS = Float32Array.BYTES_PER_ELEMENT;


// връща WebGL контекст, свързан с HTML елемент с даден id
function getContext(id)
{
	var canvas = document.getElementById(id);
	if (!canvas)
	{
		alert("Искаме canvas с id=\""+id+"\", а няма!");
		return null;
	}

	var context = canvas.getContext("webgl");
	if (!context)
	{
		context = canvas.getContext("experimental-webgl");
	}
	
	if (!context)
	{
		alert("Искаме WebGL контекст, а няма!");
	}
	
	return context;
}


// връща компилиран шейдър
function getShader(id,type)
{
	var elem = document.getElementById(id);
	var source = elem?elem.text:id;
	var shader = gl.createShader(type);

	gl.shaderSource(shader,source);
	gl.compileShader(shader);
	if (!gl.getShaderParameter(shader,gl.COMPILE_STATUS))
	{
		alert(gl.getShaderInfoLog(shader));
		return null;
	}
	
	return shader;
}


// връща готова програма
function getProgram(idv,idf)
{
	var vShader = getShader(idv,gl.VERTEX_SHADER);
	var fShader = getShader(idf,gl.FRAGMENT_SHADER);
			
	if (!vShader || !fShader) {return null;}
	
	var shaderProgram = gl.createProgram();
	gl.attachShader(shaderProgram,vShader);
	gl.attachShader(shaderProgram,fShader);
	gl.linkProgram(shaderProgram);

	if (!gl.getProgramParameter(shaderProgram,gl.LINK_STATUS))
	{
		alert(gl.getProgramInfoLog(shaderProgram));
		return null;
	}

	gl.useProgram(shaderProgram);
	return shaderProgram;
}


// намира адресите на всички глобални и атрибутни променливи
function getVariables()
{
	for (var i=0; i<gl.getProgramParameter(glprog,gl.ACTIVE_UNIFORMS); i++)
	{
		var name = gl.getActiveUniform(glprog,i).name;
		window[name] = gl.getUniformLocation(glprog,name);
	}

	for (var i=0; i<gl.getProgramParameter(glprog,gl.ACTIVE_ATTRIBUTES); i++)
	{
		var name = gl.getActiveAttrib(glprog,i).name;
		window[name] = gl.getAttribLocation(glprog,name);
	}
}



// случайно дробно число в интервал
function random(a,b)
{
	return a+(b-a)*Math.random();
}


// преобразува градуси в радиани
function radians(degrees)
{
	return degrees*Math.PI/180;
}


// синус
function sin(a)
{
	return Math.sin(a);
}


// косинус
function cos(a)
{
	return Math.cos(a);
}


// пи
var PI = Math.PI;


// задаване на група от свойства на обект
function custom(obj,props)
{
	for(var n in props) obj[n]=props[n];
	return obj;
}

// създава матрица за ортографска проекция
function orthoMatrix(width, height, near, far)
{
	var matrix = [
		2.0/width, 0, 0, 0,
		0, 2.0/height, 0, 0,
		0, 0, 2.0/(near-far), 0,
		0, 0, (far+near)/(near-far), 1];
	return new Float32Array(matrix);
}


// създава матрица за перспективна проекция
// ъгълът на зрителното поле е в градуси
function perspMatrix(angle, aspect, near, far)
{
	var fov = 1/Math.tan(radians(angle)/2);
	var matrix = [
		fov/aspect, 0, 0, 0,
		0, fov, 0, 0,
		0, 0, (far+near)/(near-far), -1,
		0, 0, 2.0*near*far/(near-far), 0];
	return new Float32Array(matrix);
}


// установява перспективна проекция
function perspective(angle,aspect,near,far)
{
	var proj = perspMatrix(angle,aspect,near,far);
	gl.uniformMatrix4fv(uProjectionMatrix,false,proj);
}


// единичен вектор
function unitVector(x)
{
	var len = 1/Math.sqrt( x[0]*x[0]+x[1]*x[1]+x[2]*x[2] );
	return [ len*x[0], len*x[1], len*x[2] ];
}


// векторно произведение на два вектора
function vectorProduct(x,y)
{
	return [
		x[1]*y[2]-x[2]*y[1],
		x[2]*y[0]-x[0]*y[2],
		x[0]*y[1]-x[1]*y[0] ];
}


// скаларно произведение на два вектора
function scalarProduct(x,y)
{
	return x[0]*y[0] + x[1]*y[1] + x[2]*y[2];
}


// вектор между две точки
function vectorPoints(x,y)
{
	return [ x[0]-y[0], x[1]-y[1], x[2]-y[2] ];
}


// генерира матрица за гледна точка, параметрите са масиви с по 3 елемента
function viewMatrix (eye, focus, up)
{
	// единичен вектор Z' по посоката на гледане
	var z = unitVector(vectorPoints(eye,focus));
	
	// единичен вектор X', перпендикулярен на Z' и на посоката нагоре
	var x = unitVector(vectorProduct(up,z));
	
	// единичен вектор Y', перпендикулярен на X' и Z'
	var y = unitVector(vectorProduct(z,x));
	
	// резултатът е тези три вектора + транслация
	var matrix = [
		x[0], y[0], z[0], 0,
		x[1], y[1], z[1], 0,
		x[2], y[2], z[2], 0,
		-scalarProduct(x,eye),
		-scalarProduct(y,eye),
		-scalarProduct(z,eye), 1 ];
	return new Float32Array(matrix);
};


// умножение на матрици
function multiplyMatrix(a, b) {
	var a00 = a[0], a01 = a[1], a02 = a[2], a03 = a[3],
		a10 = a[4], a11 = a[5], a12 = a[6], a13 = a[7],
		a20 = a[8], a21 = a[9], a22 = a[10], a23 = a[11],
		a30 = a[12], a31 = a[13], a32 = a[14], a33 = a[15];
	var out=[];
	var b0  = b[0], b1 = b[1], b2 = b[2], b3 = b[3];  
	out[0] = b0*a00 + b1*a10 + b2*a20 + b3*a30;
	out[1] = b0*a01 + b1*a11 + b2*a21 + b3*a31;
	out[2] = b0*a02 + b1*a12 + b2*a22 + b3*a32;
	out[3] = b0*a03 + b1*a13 + b2*a23 + b3*a33;

	b0 = b[4]; b1 = b[5]; b2 = b[6]; b3 = b[7];
	out[4] = b0*a00 + b1*a10 + b2*a20 + b3*a30;
	out[5] = b0*a01 + b1*a11 + b2*a21 + b3*a31;
	out[6] = b0*a02 + b1*a12 + b2*a22 + b3*a32;
	out[7] = b0*a03 + b1*a13 + b2*a23 + b3*a33;

	b0 = b[8]; b1 = b[9]; b2 = b[10]; b3 = b[11];
	out[8] = b0*a00 + b1*a10 + b2*a20 + b3*a30;
	out[9] = b0*a01 + b1*a11 + b2*a21 + b3*a31;
	out[10] = b0*a02 + b1*a12 + b2*a22 + b3*a32;
	out[11] = b0*a03 + b1*a13 + b2*a23 + b3*a33;

	b0 = b[12]; b1 = b[13]; b2 = b[14]; b3 = b[15];
	out[12] = b0*a00 + b1*a10 + b2*a20 + b3*a30;
	out[13] = b0*a01 + b1*a11 + b2*a21 + b3*a31;
	out[14] = b0*a02 + b1*a12 + b2*a22 + b3*a32;
	out[15] = b0*a03 + b1*a13 + b2*a23 + b3*a33;
	return out;
};

// създаване на матрица за нормалните вектори,
// чрез реципрочна стойност и транспозиция
function calculateNormalMatrix(a) {
	var a00 = a[0], a01 = a[1], a02 = a[2], a03 = a[3],
		a10 = a[4], a11 = a[5], a12 = a[6], a13 = a[7],
		a20 = a[8], a21 = a[9], a22 = a[10], a23 = a[11],
		a30 = a[12], a31 = a[13], a32 = a[14], a33 = a[15],

		b00 = a00 * a11 - a01 * a10,
		b01 = a00 * a12 - a02 * a10,
		b02 = a00 * a13 - a03 * a10,
		b03 = a01 * a12 - a02 * a11,
		b04 = a01 * a13 - a03 * a11,
		b05 = a02 * a13 - a03 * a12,
		b06 = a20 * a31 - a21 * a30,
		b07 = a20 * a32 - a22 * a30,
		b08 = a20 * a33 - a23 * a30,
		b09 = a21 * a32 - a22 * a31,
		b10 = a21 * a33 - a23 * a31,
		b11 = a22 * a33 - a23 * a32,

		// детерминанта
		det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

	det = 1.0 / det;

	var out=[];
	out[0] = (a11 * b11 - a12 * b10 + a13 * b09) * det;
	out[1] = (a12 * b08 - a10 * b11 - a13 * b07) * det;
	out[2] = (a10 * b10 - a11 * b08 + a13 * b06) * det;
	out[3] = 0;
	
	out[4] = (a02 * b10 - a01 * b11 - a03 * b09) * det;
	out[5] = (a00 * b11 - a02 * b08 + a03 * b07) * det;
	out[6] = (a01 * b08 - a00 * b10 - a03 * b06) * det;
	out[7] = 0;

	out[8] = (a31 * b05 - a32 * b04 + a33 * b03) * det;
	out[9] = (a32 * b02 - a30 * b05 - a33 * b01) * det;
	out[10] = (a30 * b04 - a31 * b02 + a33 * b00) * det;
	out[11] = 0;
	
	out[12] = 0;
	out[13] = 0;
	out[14] = 0;
	out[15] = 1;

	return out;
};


// установява гледна точка
function lookAt(eye,target,up)
{
	glvmat = viewMatrix(eye,target,up);
	gl.uniformMatrix4fv(uViewMatrix,false,glvmat);
}


// единична матрица
function unitMatrix()
{
	var matrix = [
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1];
	return new Float32Array(matrix);
}

// матрица за транслация по вектор V
function translateMatrix(v)
{
	var matrix = [
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		v[0], v[1], v[2], 1];
	return new Float32Array(matrix);
}


// матрица за мащабиране с вектор V
function scaleMatrix(v)
{
	var matrix = [
		v[0], 0, 0, 0,
		0, v[1], 0, 0,
		0, 0, v[2], 0,
		0, 0, 0, 1];
	return new Float32Array(matrix);
}


// матрица за въртене от X към Y около Z на ъгъл A
function zRotateMatrix(a)
{
	a = radians(a);
	var c = Math.cos(a);
	var s = Math.sin(a);
	var matrix = [
		c, s, 0, 0,
	   -s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1];
	return new Float32Array(matrix);
}


// матрица за въртене от Y към Z около X на ъгъл A
function xRotateMatrix(a)
{
	a = radians(a);
	var c = Math.cos(a);
	var s = Math.sin(a);
	var matrix = [
		1, 0, 0, 0,
		0, c, s, 0,
		0,-s, c, 0,
		0, 0, 0, 1];
	return new Float32Array(matrix);
}


// матрица за въртене от X към Z около Y на ъгъл A
function yRotateMatrix(a)
{
	a = radians(a);
	var c = Math.cos(a);
	var s = Math.sin(a);
	var matrix = [
		c, 0, s, 0,
		0, 1, 0, 0,
	   -s, 0, c, 0,
		0, 0, 0, 1];
	return new Float32Array(matrix);
}


// матрица за въртене около вектор V на ъгъл A
function rotateMatrix(a, v)
{
	a = radians(a);
	v = unitVector(v);
	
	var c = Math.cos(a);
	var s = Math.sin(a);
	
	var xx = v[0]*v[0]*(1-c);
	var xy = v[0]*v[1]*(1-c);
	var xz = v[0]*v[2]*(1-c);
	var yy = v[1]*v[1]*(1-c);
	var yz = v[1]*v[2]*(1-c);
	var zz = v[2]*v[2]*(1-c);
	
	var matrix = [
		xx+c,      xy-v[2]*s, xz+v[1]*s, 0,
		xy+v[2]*s, yy+c,      yz-v[0]*s, 0,
		xz-v[1]*s, yz+v[0]*s, zz+c,      0,
		0, 0, 0, 1];
	return new Float32Array(matrix);
}


// зарежда единичната матрица в матрицата на модела
function identity()
{
	glmatnew = true;
	glmat = new Float32Array( [1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1] );
}


// добавя транслация към матрицата на модела
function translate(v)
{
	glmatnew = true;
	glmat[12] += glmat[0]*v[0]+glmat[4]*v[1]+glmat[8]*v[2];
	glmat[13] += glmat[1]*v[0]+glmat[5]*v[1]+glmat[9]*v[2];
	glmat[14] += glmat[2]*v[0]+glmat[6]*v[1]+glmat[10]*v[2];
}


// добавя мащабиране към матрицата на модела
function scale(v)
{
	glmatnew = true;
	glmat[0] *= v[0];
	glmat[1] *= v[0];
	glmat[2] *= v[0];
	
	glmat[4] *= v[1];
	glmat[5] *= v[1];
	glmat[6] *= v[1];
	
	glmat[8] *= v[2];
	glmat[9] *= v[2];
	glmat[10] *= v[2];
}


// ако матрицата на модела е променена, изпраща я към шейдъра
function useMatrix()
{
	if (glmatnew)
	{
		glmatnew = false;
		gl.uniformMatrix4fv(uModelMatrix,false,glmat);
	}
}


// добавя въртене около X към матрицата на модела
function xRotate(a)
{
	glmatnew = true;
	
	a = radians(a);
	var s = Math.sin(a);
	var c = Math.cos(a);
	
	a = glmat[4]*s+glmat[8]*c;
	glmat[4]=glmat[4]*c-glmat[8]*s;
	glmat[8]=a;
	
	a = glmat[5]*s+glmat[9]*c;
	glmat[5]=glmat[5]*c-glmat[9]*s;
	glmat[9]=a;
	
	a = glmat[6]*s+glmat[10]*c;
	glmat[6]=glmat[6]*c-glmat[10]*s;
	glmat[10]=a;
}


// добавя въртене около Y към матрицата на модела
function yRotate(a)
{
	glmatnew = true;

	a = radians(a);
	var s = Math.sin(a);
	var c = Math.cos(a);
	
	a = glmat[0]*s+glmat[8]*c;
	glmat[0]=glmat[0]*c-glmat[8]*s;
	glmat[8]=a;
	
	a = glmat[1]*s+glmat[9]*c;
	glmat[1]=glmat[1]*c-glmat[9]*s;
	glmat[9]=a;
	
	a = glmat[2]*s+glmat[10]*c;
	glmat[2]=glmat[2]*c-glmat[10]*s;
	glmat[10]=a;
}


// добавя въртене около Z към матрицата на модела
function zRotate(a)
{
	glmatnew = true;

	a = radians(a);
	var s = Math.sin(a);
	var c = Math.cos(a);
	
	a = glmat[0]*s+glmat[4]*c;
	glmat[0]=glmat[0]*c-glmat[4]*s;
	glmat[4]=a;
	
	a = glmat[1]*s+glmat[5]*c;
	glmat[1]=glmat[1]*c-glmat[5]*s;
	glmat[5]=a;
	
	a = glmat[2]*s+glmat[6]*c;
	glmat[2]=glmat[2]*c-glmat[6]*s;
	glmat[6]=a;
}


// добавя текущата матрица на модела в стека за матрици
function pushMatrix()
{
	var mat = new Float32Array(glmat.length);
	mat.set(glmat);
	glstack.push(mat);
}


// извлича матрица на модела от стека за матрици
// при празен стек връща единичната матрица
function popMatrix()
{
	glmatnew = true;
	if (glstack.length)
		glmat = glstack.pop();
	else
		identity();
}


// клонира матрица като копира всичките ѝ стойности
function cloneMatrix(a)
{
	var b = new Float32Array(a.length);
	b.set(a);
	return b;
}



// каноничен куб - конструктор
CanonicalCube = function()
{	
	// върхове
	var v = [ [+0.5,-0.5,-0.5], [+0.5,+0.5,-0.5],
			  [-0.5,+0.5,-0.5], [-0.5,-0.5,-0.5],
			  [+0.5,-0.5,+0.5], [+0.5,+0.5,+0.5],
			  [-0.5,+0.5,+0.5], [-0.5,-0.5,+0.5] ];
	// нормални вектори
	var n = [ [1,0,0], [-1,0,0],
			  [0,1,0], [0,-1,0],
			  [0,0,1], [0,0,-1] ];
	// общ списък връх-нормала
	var data = [].concat(
			  v[0],n[0],0,0, v[1],n[0],1,0, v[4],n[0],0,1, // предна стена
			  v[4],n[0],0,1, v[1],n[0],1,0, v[5],n[0],1,1,
			  v[6],n[1],0,1, v[2],n[1],0,0, v[7],n[1],1,1, // задна стена
			  v[7],n[1],1,1, v[2],n[1],0,0, v[3],n[1],1,0, 
			  v[5],n[2],0,1, v[1],n[2],0,0, v[6],n[2],1,1, // дясна стена 
			  v[6],n[2],1,1, v[1],n[2],0,0, v[2],n[2],1,0, 
			  v[4],n[3],1,1, v[7],n[3],0,1, v[0],n[3],1,0, // лява стена 
			  v[0],n[3],1,0, v[7],n[3],0,1, v[3],n[3],0,0, 
			  v[4],n[4],0,0, v[5],n[4],1,0, v[7],n[4],0,1, // горна стена
			  v[7],n[4],0,1, v[5],n[4],1,0, v[6],n[4],1,1, 
			  v[0],n[5],0,0, v[3],n[5],0,1, v[1],n[5],1,0, // долна стена 
			  v[1],n[5],1,0, v[3],n[5],0,1, v[2],n[5],1,1 );
	// локална променлива за инстанцията с WebGL буфер
	var buf = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,buf);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(data), gl.STATIC_DRAW);
	// запомняме буфера в текущата инстанция
	this.buf = buf;
}

// каноничен куб - метод за рисуване
CanonicalCube.prototype.draw = function(texture)
{	
	// активираме буфера, създаден от конструктора
	gl.bindBuffer(gl.ARRAY_BUFFER,this.buf);
	// казваме къде са координатите
	gl.enableVertexAttribArray(aXYZ);
	gl.vertexAttribPointer(aXYZ,3,gl.FLOAT,false,8*FLOATS,0*FLOATS);
	// казваме къде са нормалите
	gl.enableVertexAttribArray(aNormal);
	gl.vertexAttribPointer(aNormal,3,gl.FLOAT,false,8*FLOATS,3*FLOATS);
	// казваме къде са текстурите
	if (gl.isTexture(texture))
	{
		gl.bindTexture(gl.TEXTURE_2D,texture);
		gl.enableVertexAttribArray(aST);
		gl.vertexAttribPointer(aST,2,gl.FLOAT,false,8*FLOATS,6*FLOATS);
	}
	else
	{
		if (texture) gl.disableVertexAttribArray(aST);
	}
	// рисуваме
	gl.drawArrays(gl.TRIANGLES,0,36);
}

var canonicalCube;

// куб - конструктор с параметри център и размер
Cube = function(center,size)
{
	// съхраняваме центъра и размера на куба
	this.center = center;
	this.size = size;
	this.color = [1,0.75,0];
	this.offset = undefined;
	this.rot = undefined;
	this.texture = undefined; // неизвестна текстура
	this.texMatrix = new Float32Array([1,0,0,0,1,0,0,0,1]); // текстурна матрица
	// създаваме еднократно канонична инстанция
	if (!canonicalCube)
		canonicalCube = new CanonicalCube();
}

// куб - рисуване
Cube.prototype.draw = function()
{
	pushMatrix(); // запомняме матрицата
	gl.vertexAttrib3fv(aColor,this.color); // подаваме цвета
	translate(this.center); // мястото
	if (this.rot)
	{
		if (this.rot[0]) zRotate(this.rot[0]);	// хоризонтален ъгъл
		if (this.rot[1]) yRotate(this.rot[1]);	// вертикален ъгъл
		if (this.rot[2]) xRotate(this.rot[2]);	// вертикален ъгъл
		if (this.rot[3]) zRotate(this.rot[3]);	// осев ъгъл
	}
	scale([this.size,this.size,this.size]); // и размера
	if (this.offset) translate(this.offset); // и отместването
	useMatrix();
	if (this.texture) gl.uniformMatrix3fv(uTexMatrix,false,this.texMatrix);
	canonicalCube.draw(this.texture); // самото рисуване
	popMatrix(); // възстановяваме матрицата
}



// кубоид - конструктор с параметри център и размер
Cuboid = function(center,size)
{
	// съхраняваме центъра и размера на куба
	this.center = center;
	this.size = size;
	this.color = [1,0.75,0];
	this.offset = undefined;
	this.rot = undefined;
	this.texture = undefined; // неизвестна текстура
	this.texMatrix = new Float32Array([1,0,0,0,1,0,0,0,1]); // текстурна матрица
	// създаваме еднократно канонична инстанция
	if (!canonicalCube)
		canonicalCube = new CanonicalCube();
}

// кубоид - рисуване
Cuboid.prototype.draw = function()
{
	pushMatrix(); // запомняме матрицата
	gl.vertexAttrib3fv(aColor,this.color); // подаваме цвета
	translate(this.center); // мястото
	if (this.rot)
	{
		if (this.rot[0]) zRotate(this.rot[0]);	// хоризонтален ъгъл
		if (this.rot[1]) yRotate(this.rot[1]);	// вертикален ъгъл
		if (this.rot[2]) xRotate(this.rot[2]);	// вертикален ъгъл
		if (this.rot[3]) zRotate(this.rot[3]);	// осев ъгъл
	}
	scale(this.size); // и размера
	if (this.offset) translate(this.offset); // и отместването
	useMatrix();
	gl.uniformMatrix3fv(uTexMatrix,false,this.texMatrix);
	canonicalCube.draw(this.texture); // самото рисуване
	popMatrix(); // възстановяваме матрицата
}



// канонична пирамида - конструктор
CanonicalPyramid = function(n)
{	
	// текущ ъгъл и ъглова разлика
	var a = 0, dA = 2*Math.PI/n;

	// генериране на основата като ветрило
	var data = [0,0,0, 0,0,-1];
	for (var i=0; i<=n; i++)
	{ 
		data.push(Math.cos(a),Math.sin(a),0,0,0,-1);
		a += dA;
	}

	// генериране на околните стени
	a = 0;
	var nZ = Math.cos(Math.PI/n); // височина на нормалния вектор
	for (var i=0; i<=n; i++)
	{ 
		// нормален вектор (няма нужда да е единичен, в щейдъра се нормализира)
		var N = [Math.cos(a+dA/2),Math.sin(a+dA/2),0/*nZ*/];
		data.push(0,0,1,N[0],N[1],nZ);
		data.push(Math.cos(a),Math.sin(a),0,N[0],N[1],0);
		data.push(Math.cos(a+dA),Math.sin(a+dA),0,N[0],N[1],0);
		a += dA;
	}
	
	var buf = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,buf);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(data), gl.STATIC_DRAW);
	
	// запомняме n и буфера
	this.n = n;
	this.buf = buf;
}

// канонична пирамида - метод за рисуване
CanonicalPyramid.prototype.draw = function(hollow)
{	
	gl.bindBuffer(gl.ARRAY_BUFFER,this.buf);
	// върхове
	gl.enableVertexAttribArray(aXYZ);
	gl.vertexAttribPointer(aXYZ,3,gl.FLOAT,false,6*FLOATS,0*FLOATS);
	// нормали
	gl.enableVertexAttribArray(aNormal);
	gl.vertexAttribPointer(aNormal,3,gl.FLOAT,false,6*FLOATS,3*FLOATS);
	// рисуваме основата
	if (!hollow)
	{
		gl.drawArrays(gl.TRIANGLE_FAN,0,this.n+2);
	}
	// рисуваме околните стени
	gl.drawArrays(gl.TRIANGLES,this.n+2,3*this.n);
}

// масив от канонични пирамиди
var canonicalPyramid = [];

// пирамида - конструктор с параметри център, размер на основата, височина и брой стени
Pyramid = function(center,size,height,n)
{
	this.center = center;
	this.size = size;
	this.height = height;
	this.n = n;
	this.color = [1,0.75,0];
	this.offset = undefined;
	this.hollow = false;
	this.rot = undefined;
	// създаваме еднократно канонична пирамида
	if (!canonicalPyramid[n])
		canonicalPyramid[n] = new CanonicalPyramid(n);
}

// пирамида - рисуване
Pyramid.prototype.draw = function()
{
	pushMatrix();
	gl.vertexAttrib3fv(aColor,this.color);
	translate(this.center);
	if (this.rot)
	{
		if (this.rot[0]) zRotate(this.rot[0]);	// хоризонтален ъгъл
		if (this.rot[1]) yRotate(this.rot[1]);	// вертикален ъгъл
		if (this.rot[2]) xRotate(this.rot[2]);	// вертикален ъгъл
		if (this.rot[3]) zRotate(this.rot[3]);	// осев ъгъл
	}
	scale([this.size,this.size,this.height]);
	if (this.offset) translate(this.offset); // и отместването
	useMatrix();
	canonicalPyramid[this.n].draw(this.hollow);
	popMatrix();
}


// каноничен конус - конструктор
CanonicalCone = function(n)
{	
	// текущ ъгъл и ъглова разлика
	var a = 0, dA = 2*Math.PI/n;

	// генериране на основата като ветрило
	var data = [0,0,0, 0,0,-1];
	for (var i=0; i<=n; i++)
	{ 
		data.push(Math.cos(a),Math.sin(a),0,0,0,-1);
		a += dA;
	}

	// генериране на околните стени
	a = 0;
	//var nZ = Math.cos(Math.PI/n); // височина на нормалния вектор
	for (var i=0; i<=n; i++)
	{ 
		// нормален вектор (няма нужда да е единичен, в щейдъра се нормализира)
		data.push(0,0,1,0,0,1/*N[0],N[1],N[2]*/);
		data.push(Math.cos(a),Math.sin(a),0,Math.cos(a),Math.sin(a),0/*Nz*/);
		a += dA;
		data.push(Math.cos(a),Math.sin(a),0,Math.cos(a),Math.sin(a),0/*Nz*/);
	}
	
	var buf = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,buf);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(data), gl.STATIC_DRAW);
	
	// запомняме n и буфера
	this.n = n;
	this.buf = buf;
}

// канонична пирамида - метод за рисуване
CanonicalCone.prototype.draw = function(hollow)
{	
	gl.bindBuffer(gl.ARRAY_BUFFER,this.buf);
	// върхове
	gl.enableVertexAttribArray(aXYZ);
	gl.vertexAttribPointer(aXYZ,3,gl.FLOAT,false,6*FLOATS,0*FLOATS);
	// нормали
	gl.enableVertexAttribArray(aNormal);
	gl.vertexAttribPointer(aNormal,3,gl.FLOAT,false,6*FLOATS,3*FLOATS);
	// рисуваме основата
	if (!hollow)
	{
		gl.drawArrays(gl.TRIANGLE_FAN,0,this.n+2);
	}
	// рисуваме околните стени
	gl.drawArrays(gl.TRIANGLES,this.n+2,3*this.n);
}

// масив от канонични конуси
var canonicalCone = [];

// конус - конструктор с параметри център, размер на основата, височина
var CONE_SIDES = 32;
Cone = function(center,size,height)
{
	this.center = center;
	this.size = size;
	this.height = height;
	this.n = CONE_SIDES;
	this.color = [1,0.75,0];
	this.offset = undefined;
	this.hollow = false;
	this.rot = undefined;
	// създаваме еднократно канонична пирамида
	if (!canonicalCone[this.n])
		canonicalCone[this.n] = new CanonicalCone(this.n);
}

// конус - рисуване
Cone.prototype.draw = function()
{
	pushMatrix();
	gl.vertexAttrib3fv(aColor,this.color);
	translate(this.center);
	if (this.rot)
	{
		if (this.rot[0]) zRotate(this.rot[0]);	// хоризонтален ъгъл
		if (this.rot[1]) yRotate(this.rot[1]);	// вертикален ъгъл
		if (this.rot[2]) xRotate(this.rot[2]);	// вертикален ъгъл
		if (this.rot[3]) zRotate(this.rot[3]);	// осев ъгъл
	}
	scale([this.size,this.size,this.height]);
	if (this.offset) translate(this.offset); // и отместването
	useMatrix();
	canonicalCone[this.n].draw(this.hollow);
	popMatrix();
}



// канонична призма - конструктор
CanonicalPrism = function(n)
{	
	// текущ ъгъл и ъглова разлика
	var a = 0, dA = 2*Math.PI/n;

	// генериране на долната основа като ветрило
	var data = [0,0,0, 0,0,-1];
	for (var i=0; i<=n; i++)
	{ 
		data.push(Math.cos(a),Math.sin(a),0,0,0,-1);
		a += dA;
	}

	// генериране на горната основа като ветрило
	data.push(0,0,1, 0,0,1);
	for (var i=0; i<=n; i++)
	{ 
		data.push(Math.cos(a),Math.sin(a),1,0,0,1);
		a += dA;
	}

	// генериране на околните стени
	a = 0;
	var nZ = Math.cos(Math.PI/n); // височина на нормалния вектор
	for (var i=0; i<=n; i++)
	{ 
		var N = [Math.cos(a+dA/2),Math.sin(a+dA/2)];
		data.push(Math.cos(a),Math.sin(a),1,N[0],N[1],0);
		data.push(Math.cos(a),Math.sin(a),0,N[0],N[1],0);
		data.push(Math.cos(a+dA),Math.sin(a+dA),0,N[0],N[1],0);
		data.push(Math.cos(a+dA),Math.sin(a+dA),1,N[0],N[1],0);
		data.push(Math.cos(a+dA),Math.sin(a+dA),0,N[0],N[1],0);
		data.push(Math.cos(a),Math.sin(a),1,N[0],N[1],0);
		a += dA;
	}
	
	var buf = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,buf);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(data), gl.STATIC_DRAW);
	
	// запомняме n и буфера
	this.n = n;
	this.buf = buf;
}

// канонична призма - метод за рисуване
CanonicalPrism.prototype.draw = function(hollow)
{	
	gl.bindBuffer(gl.ARRAY_BUFFER,this.buf);
	// върхове
	gl.enableVertexAttribArray(aXYZ);
	gl.vertexAttribPointer(aXYZ,3,gl.FLOAT,false,6*FLOATS,0*FLOATS);
	// нормали
	gl.enableVertexAttribArray(aNormal);
	gl.vertexAttribPointer(aNormal,3,gl.FLOAT,false,6*FLOATS,3*FLOATS);
	// рисуваме долната и горната основа
	if (!hollow)
	{
		gl.drawArrays(gl.TRIANGLE_FAN,0,this.n+2);
		gl.drawArrays(gl.TRIANGLE_FAN,this.n+2,this.n+2);
	}
	// рисуваме околните стени
	gl.drawArrays(gl.TRIANGLES,2*this.n+4,6*this.n);
}

// масив от канонични призми
var canonicalPrism = [];

// призма - конструктор с параметри център, размер на основата, височина и брой стени
Prism = function(center,size,height,n)
{
	this.center = center;
	this.size = size;
	this.height = height;
	this.n = n;
	this.color = [1,0.75,0];
	this.offset = undefined;
	this.hollow = false;
	this.rot = undefined;
	// създаваме еднократно канонична призма
	if (!canonicalPrism[n])
		canonicalPrism[n] = new CanonicalPrism(n);
}

// призма - рисуване
Prism.prototype.draw = function()
{
	pushMatrix();
	gl.vertexAttrib3fv(aColor,this.color);
	translate(this.center);
	if (this.rot)
	{
		if (this.rot[0]) zRotate(this.rot[0]);	// хоризонтален ъгъл
		if (this.rot[1]) yRotate(this.rot[1]);	// вертикален ъгъл
		if (this.rot[2]) xRotate(this.rot[2]);	// вертикален ъгъл
		if (this.rot[3]) zRotate(this.rot[3]);	// осев ъгъл
	}
	scale([this.size,this.size,this.height]);
	if (this.offset) translate(this.offset); // и отместването
	useMatrix();
	canonicalPrism[this.n].draw(this.hollow);
	popMatrix();
}



// каноничен цилиндър - конструктор
CanonicalCylinder = function(n)
{	
	// текущ ъгъл и ъглова разлика
	var a = 0, dA = 2*Math.PI/n;

	// генериране на долната основа като ветрило
	var data = [0,0,0, 0,0,-1, 0.5,0.5];
	for (var i=0; i<=n; i++)
	{ 
		data.push(Math.cos(a),Math.sin(a),0,0,0,-1, 0.5+0.5*Math.cos(a),0.5+0.5*Math.sin(a));
		a += dA;
	}

	// генериране на горната основа като ветрило
	data.push(0,0,1, 0,0,1, 0.5,0.5);
	for (var i=0; i<=n; i++)
	{ 
		data.push(Math.cos(a),Math.sin(a),1,0,0,1, 0.5+0.5*Math.cos(a),0.5+0.5*Math.sin(a));
		a += dA;
	}

	// генериране на околните стени
	a = 0;
	var nZ = Math.cos(Math.PI/n); // височина на нормалния вектор
	for (var i=0; i<=n; i++)
	{ 
		var N = [Math.cos(a),Math.sin(a)]; // нормала към един отвес
		var M = [Math.cos(a+dA),Math.sin(a+dA)]; // нормала към следващия отвес
		data.push(Math.cos(a),Math.sin(a),1,N[0],N[1],0, i/n,1);
		data.push(Math.cos(a),Math.sin(a),0,N[0],N[1],0, i/n,0);
		data.push(Math.cos(a+dA),Math.sin(a+dA),0,M[0],M[1],0, (i+1)/n,0);
		data.push(Math.cos(a+dA),Math.sin(a+dA),1,M[0],M[1],0, (i+1)/n,1);
		data.push(Math.cos(a+dA),Math.sin(a+dA),0,M[0],M[1],0, (i+1)/n,0);
		data.push(Math.cos(a),Math.sin(a),1,N[0],N[1],0, i/n,1);
		a += dA;
	}
	
	var buf = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,buf);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(data), gl.STATIC_DRAW);
	
	// запомняме n и буфера
	this.n = n;
	this.buf = buf;
}

// каноничен цилиндър - метод за рисуване
CanonicalCylinder.prototype.draw = function(hollow, texture, texMatrix, texMatrixBase)
{	
	gl.bindBuffer(gl.ARRAY_BUFFER,this.buf);
	// върхове
	gl.enableVertexAttribArray(aXYZ);
	gl.vertexAttribPointer(aXYZ,3,gl.FLOAT,false,8*FLOATS,0*FLOATS);
	// нормали
	gl.enableVertexAttribArray(aNormal);
	gl.vertexAttribPointer(aNormal,3,gl.FLOAT,false,8*FLOATS,3*FLOATS);
	// казваме къде са текстурите
	if (gl.isTexture(texture))
	{
		gl.bindTexture(gl.TEXTURE_2D,texture);
		gl.enableVertexAttribArray(aST);
		gl.vertexAttribPointer(aST,2,gl.FLOAT,false,8*FLOATS,6*FLOATS);
	}
	else
	{
		gl.disableVertexAttribArray(aST);
	}
	// рисуваме долната и горната основа
	if (!hollow)
	{
		gl.uniformMatrix3fv(uTexMatrix,false,texMatrixBase);
		gl.drawArrays(gl.TRIANGLE_FAN,0,this.n+2);
		gl.drawArrays(gl.TRIANGLE_FAN,this.n+2,this.n+2);
	}
	// рисуваме околните стени
	gl.uniformMatrix3fv(uTexMatrix,false,texMatrix);
	gl.drawArrays(gl.TRIANGLES,2*this.n+4,6*this.n);
}

// масив от канонични цилиндри
var canonicalCylinder = [];

// цилиндър - конструктор с параметри център, размер на основата, височина и брой стени
var CYLINDER_SIDES = 32;
Cylinder = function(center,size,height)
{
	this.center = center;
	this.size = size;
	this.height = height;
	this.n = CYLINDER_SIDES;
	this.color = [1,0.75,0];
	this.offset = undefined;
	this.hollow = false;
	this.rot = undefined;
	this.texture = undefined; // неизвестна текстура
	this.texMatrix = new Float32Array([1,0,0,0,1,0,0,0,1]); // текстурна матрица
	this.texMatrixBase = new Float32Array([1,0,0,0,1,0,0,0,1]); // текстурна матрица на основите
	// създаваме еднократно канонична призма
	if (!canonicalCylinder[this.n])
		canonicalCylinder[this.n] = new CanonicalCylinder(this.n);
}

// цилиндър - рисуване
Cylinder.prototype.draw = function()
{
	pushMatrix();
	gl.vertexAttrib3fv(aColor,this.color);
	translate(this.center);
	if (this.rot)
	{
		if (this.rot[0]) zRotate(this.rot[0]);	// хоризонтален ъгъл
		if (this.rot[1]) yRotate(this.rot[1]);	// вертикален ъгъл
		if (this.rot[2]) xRotate(this.rot[2]);	// вертикален ъгъл
		if (this.rot[3]) zRotate(this.rot[3]);	// осев ъгъл
	}
	scale([this.size,this.size,this.height]);
	if (this.offset) translate(this.offset); // и отместването
	useMatrix();
	canonicalCylinder[this.n].draw(this.hollow,this.texture,this.texMatrix,this.texMatrixBase);
	popMatrix();
}

// пресечен коноид - конструктор
Conoid = function(center,size,ratio)
{
	this.center = center;
	this.size = size;
	this.n = CONE_SIDES;
	this.color = [1,0.75,0];
	this.offset = undefined;
	this.hollow = false;
	this.rot = undefined;

	// заради ratio пресеченият коноид няма канонична форма
	var a = 0, dA = 2*Math.PI/this.n;
	// генериране на долната основа като ветрило
	var data = [0,0,0, 0,0,-1];
	for (var i=0; i<=this.n; i++)
	{ 
		data.push(Math.cos(a),Math.sin(a),0,0,0,-1);
		a += dA;
	}
	// генериране на горната основа като ветрило
	data.push(0,0,1, 0,0,1);
	for (var i=0; i<=this.n; i++)
	{ 
		data.push(ratio[0]*Math.cos(a),ratio[1]*Math.sin(a),1,0,0,1);
		a += dA;
	}
	// генериране на околните стени
	a = 0;
	var nZ = Math.cos(Math.PI/this.n); // височина на нормалния вектор
	for (var i=0; i<=this.n; i++)
	{ 
		var N = [Math.cos(a),Math.sin(a)]; // нормала към един отвес
		var M = [Math.cos(a+dA),Math.sin(a+dA)]; // нормала към следващия отвес
		data.push(ratio[0]*Math.cos(a),ratio[1]*Math.sin(a),1,N[0],N[1],0);
		data.push(Math.cos(a),Math.sin(a),0,N[0],N[1],0);
		data.push(Math.cos(a+dA),Math.sin(a+dA),0,M[0],M[1],0);
		data.push(ratio[0]*Math.cos(a+dA),ratio[1]*Math.sin(a+dA),1,M[0],M[1],0);
		data.push(Math.cos(a+dA),Math.sin(a+dA),0,M[0],M[1],0);
		data.push(ratio[0]*Math.cos(a),ratio[1]*Math.sin(a),1,N[0],N[1],0);
		a += dA;
	}
	var buf = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,buf);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(data), gl.STATIC_DRAW);
	this.buf = buf;
}

// // пресечен коноид - рисуване
Conoid.prototype.draw = function()
{
	pushMatrix();
	gl.vertexAttrib3fv(aColor,this.color);
	translate(this.center);
	if (this.rot)
	{
		if (this.rot[0]) zRotate(this.rot[0]);	// хоризонтален ъгъл
		if (this.rot[1]) yRotate(this.rot[1]);	// вертикален ъгъл
		if (this.rot[2]) xRotate(this.rot[2]);	// вертикален ъгъл
		if (this.rot[3]) zRotate(this.rot[3]);	// осев ъгъл
	}
	scale(this.size);
	if (this.offset) translate(this.offset); // и отместването
	useMatrix();
		// рисуване на пресечен конус
		gl.bindBuffer(gl.ARRAY_BUFFER,this.buf);
		gl.enableVertexAttribArray(aXYZ);
		gl.vertexAttribPointer(aXYZ,3,gl.FLOAT,false,6*FLOATS,0*FLOATS);
		gl.enableVertexAttribArray(aNormal);
		gl.vertexAttribPointer(aNormal,3,gl.FLOAT,false,6*FLOATS,3*FLOATS);
		if (!this.hollow)
		{
			gl.drawArrays(gl.TRIANGLE_FAN,0,this.n+2);
			gl.drawArrays(gl.TRIANGLE_FAN,this.n+2,this.n+2);
		}
		gl.drawArrays(gl.TRIANGLES,2*this.n+4,6*this.n);
	popMatrix();
}

// канонична сфера - конструктор
CanonicalSphere = function(n)
{	
	n = 2*Math.floor(n/2);
	function dataPush(a,b,s,t)
	{	// координати на точка и нормален вектор, текстурни координати
		data.push(
			Math.cos(a)*Math.cos(b),
			Math.sin(a)*Math.cos(b),
			Math.sin(b),
			s, t );
	}
	
	var data = [];
	
	// генериране на хоризонтални ленти
	var b = -Math.PI/2, dB = 2*Math.PI/n;
	for (var bi=0; bi<n/2; bi++)
	{
		// генериране на една лента
		var a = 0, dA = 2*Math.PI/n;
		for (var ai=0; ai<=n; ai++)
		{
			dataPush(a,b,ai/n,bi/(n/2));
			dataPush(a,b+dB,ai/n,(bi+1)/(n/2));
			a += dA;
		}
		b += dB;
	}

	var buf = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,buf);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(data), gl.STATIC_DRAW);
	
	// запомняме n и буфера
	this.n = n;
	this.buf = buf;
}

// канонична сфера - метод за рисуване
CanonicalSphere.prototype.draw = function(texture)
{	
	gl.bindBuffer(gl.ARRAY_BUFFER,this.buf);
	// върхове
	gl.enableVertexAttribArray(aXYZ);
	gl.vertexAttribPointer(aXYZ,3,gl.FLOAT,false,5*FLOATS,0*FLOATS);
	// нормали
	gl.enableVertexAttribArray(aNormal);
	gl.vertexAttribPointer(aNormal,3,gl.FLOAT,false,5*FLOATS,0*FLOATS);
	// казваме къде са текстурите
	if (gl.isTexture(texture))
	{
		gl.bindTexture(gl.TEXTURE_2D,texture);
		gl.enableVertexAttribArray(aST);
		gl.vertexAttribPointer(aST,2,gl.FLOAT,false,5*FLOATS,3*FLOATS);
	}
	else
	{
		if (this.texture) gl.disableVertexAttribArray(aST);
	}
	// рисуваме n ленти
	for (var i=0; i<this.n/2; i++)
		gl.drawArrays(gl.TRIANGLE_STRIP,2*(this.n+1)*i,2*(this.n+1));
}

// масив от канонични сфери
var canonicalSphere = [];

// сфера - конструктор с параметри център, радиус и n
var SPHERE_SIDES = 32;
Sphere = function(center,size)
{
	this.center = center;
	this.size = size;
	this.n = SPHERE_SIDES;
	this.color = [1,0.75,0];
	this.offset = undefined;
	this.rot = undefined;
	this.texture = undefined; // неизвестна текстура
	this.texMatrix = new Float32Array([1,0,0,0,1,0,0,0,1]); // текстурна матрица
	// създаваме еднократно канонична сфера
	if (!canonicalSphere[this.n])
		canonicalSphere[this.n] = new CanonicalSphere(this.n);
}

// сфера - рисуване
Sphere.prototype.draw = function()
{
	pushMatrix();
	gl.vertexAttrib3fv(aColor,this.color);
	translate(this.center);
	if (this.rot)
	{
		if (this.rot[0]) zRotate(this.rot[0]);	// хоризонтален ъгъл
		if (this.rot[1]) yRotate(this.rot[1]);	// вертикален ъгъл
		if (this.rot[2]) xRotate(this.rot[2]);	// вертикален ъгъл
		if (this.rot[3]) zRotate(this.rot[3]);	// осев ъгъл
	}
	scale([this.size,this.size,this.size]);
	if (this.offset) translate(this.offset); // и отместването
	useMatrix();
	if (this.texture) gl.uniformMatrix3fv(uTexMatrix,false,this.texMatrix);
	canonicalSphere[this.n].draw(this.texture,this.texMatrix);
	popMatrix();
}

// сфероид - конструктор
Spheroid = function(center,size)
{
	this.center = center;
	this.size = size;
	this.n = SPHERE_SIDES;
	this.color = [1,0.75,0];
	this.offset = undefined;
	this.rot = undefined;
	// създаваме еднократно канонична сфера
	if (!canonicalSphere[this.n])
		canonicalSphere[this.n] = new CanonicalSphere(this.n);
}

// сфероид - рисуване
Spheroid.prototype.draw = function()
{
	pushMatrix();
	gl.vertexAttrib3fv(aColor,this.color);
	translate(this.center);
	if (this.rot)
	{
		if (this.rot[0]) zRotate(this.rot[0]);	// хоризонтален ъгъл
		if (this.rot[1]) yRotate(this.rot[1]);	// вертикален ъгъл
		if (this.rot[2]) xRotate(this.rot[2]);	// вертикален ъгъл
		if (this.rot[3]) zRotate(this.rot[3]);	// осев ъгъл
	}
	scale(this.size);
	if (this.offset) translate(this.offset); // и отместването
	useMatrix();
	
	gl.uniform1i(uUseNormalMatrix,true);
	var nmat = calculateNormalMatrix(multiplyMatrix(glvmat,glmat));
	gl.uniformMatrix4fv(uNormalMatrix,false,nmat);
	canonicalSphere[this.n].draw();
	gl.uniform1i(uUseNormalMatrix,false);

	popMatrix();
}

// каноничен икосаедър - конструктор
CanonicalIcosahedron = function(n)
{	
	var data = [];
	
	// генерира триъгълник, смята нормалния
	// вектор чрез векторно произведение
	function triangle(p1,p2,p3)
	{
		var u = vectorPoints(p2,p1);
		var v = vectorPoints(p3,p1);
		var norm = unitVector(vectorProduct(u,v));
		data.push( p1[0], p1[1], p1[2], norm[0], norm[1], norm[2] );
		data.push( p2[0], p2[1], p2[2], norm[0], norm[1], norm[2] );
		data.push( p3[0], p3[1], p3[2], norm[0], norm[1], norm[2] );
	}
	
	// златното сечение 1.618...
	var f = (1+Math.sqrt(5))/2;

	// триъгълници - стени на икосаедъра
	triangle([ 0, 1, f], [ 1, f, 0], [-1, f, 0]);	// десен горен
	triangle([ 0, 1,-f], [-1, f, 0], [ 1, f, 0]);	// десен долен
	triangle([ 0,-1, f], [-1,-f, 0], [ 1,-f, 0]);	// ляв горен
	triangle([ 0,-1,-f], [ 1,-f, 0], [-1,-f, 0]);	// ляв долен

	triangle([ 1, f, 0], [ f, 0, 1], [ f, 0,-1]);	// предни и задни
	triangle([ 1,-f, 0], [ f, 0,-1], [ f, 0, 1]);
	triangle([-1, f, 0], [-f, 0,-1], [-f, 0, 1]);
	triangle([-1,-f, 0], [-f, 0, 1], [-f, 0,-1]);

	triangle([ f, 0, 1], [ 0, 1, f], [ 0,-1, f]);	// горни и долни
	triangle([-f, 0, 1], [ 0,-1, f], [ 0, 1, f]);
	triangle([ f, 0,-1], [ 0,-1,-f], [ 0, 1,-f]);
	triangle([-f, 0,-1], [ 0, 1,-f], [ 0,-1,-f]);

	triangle([ 0, 1, f], [ f, 0, 1], [ 1, f, 0]);	// горни ъглови 
	triangle([ 0, 1, f], [-1, f, 0], [-f, 0, 1]);
	triangle([ 0,-1, f], [ 1,-f, 0], [ f, 0, 1]); 
	triangle([ 0,-1, f], [-f, 0, 1], [-1,-f, 0]);
	
	triangle([ 0, 1,-f], [ 1, f, 0], [ f, 0,-1]);	// долни ъглови 
	triangle([ 0, 1,-f], [-f, 0,-1], [-1, f, 0]);
	triangle([ 0,-1,-f], [ f, 0,-1], [ 1,-f, 0]); 
	triangle([ 0,-1,-f], [-1,-f, 0], [-f, 0,-1]);
	
	var buf = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,buf);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(data), gl.STATIC_DRAW);
	
	// запомняме буфера
	this.buf = buf;
}

// икосаедър - метод за рисуване
CanonicalIcosahedron.prototype.draw = function()
{	
	gl.bindBuffer(gl.ARRAY_BUFFER,this.buf);
	// върхове
	gl.enableVertexAttribArray(aXYZ);
	gl.vertexAttribPointer(aXYZ,3,gl.FLOAT,false,6*FLOATS,0*FLOATS);
	// нормали
	gl.enableVertexAttribArray(aNormal);
	gl.vertexAttribPointer(aNormal,3,gl.FLOAT,false,6*FLOATS,3*FLOATS);
	// рисуваме 20 триъгълника
	gl.drawArrays(gl.TRIANGLES,0,3*20);
}

var canonicalIcosahedron;

// икосахедрон - конструктор
Icosahedron = function(center,size)
{
	this.center = center;
	this.size = size;
	this.color = [1,0.75,0];
	this.offset = undefined;
	this.rot = undefined;
	// създаваме еднократно каноничен икосаедър
	if (!canonicalIcosahedron)
		canonicalIcosahedron = new CanonicalIcosahedron();
}

// икосаедър - рисуване
Icosahedron.prototype.draw = function()
{
	pushMatrix();
	gl.vertexAttrib3fv(aColor,this.color);
	translate(this.center);
	if (this.rot)
	{
		if (this.rot[0]) zRotate(this.rot[0]);	// хоризонтален ъгъл
		if (this.rot[1]) yRotate(this.rot[1]);	// вертикален ъгъл
		if (this.rot[2]) xRotate(this.rot[2]);	// вертикален ъгъл
		if (this.rot[3]) zRotate(this.rot[3]);	// осев ъгъл
	}
	scale([this.size,this.size,this.size]);
	if (this.offset) translate(this.offset); // и отместването
	useMatrix();
	canonicalIcosahedron.draw();
	popMatrix();
}

var GEODESIC_SIDES = 3;

// масив от различни степени на детайлност
var canonicalGeodesicSphere = [];

// канонична геодезична сфера - конструктор
CanonicalGeodesicSphere = function(level)
{	
	var data = [];
	var n = 0;
	
	// средна точка на отсечка
	function mid(p,q)
	{
		return [(p[0]+q[0])/2,(p[1]+q[1])/2,(p[2]+q[2])/2];
	}
	
	function triangle(p1,p2,p3,level)
	{
		if (level)
		{	// ако не сме достигнали границата на раздробяване,
			//  делим триъгълника на 4 по-малки триъгълника
			var m12 = mid(p1,p2);
			var m23 = mid(p2,p3);
			var m31 = mid(p3,p1);
			level--;
			triangle(p1,m12,m31,level);
			triangle(m12,p2,m23,level);
			triangle(m31,m23,p3,level);
			triangle(m12,m23,m31,level);
		}
		else
		{	// стигнали сме границата на раздробяване,
			// генерираме триъгълника
			p1 = unitVector(p1);
			p2 = unitVector(p2);
			p3 = unitVector(p3);
			data.push( p1[0], p1[1], p1[2] );
			data.push( p2[0], p2[1], p2[2] );
			data.push( p3[0], p3[1], p3[2] );
			n++;
		}
	}
	
	// златното сечение 1.618...
	var f = (1+Math.sqrt(5))/2;

	// триъгълници - стени на икосаедър
	triangle([ 0, 1, f], [ 1, f, 0], [-1, f, 0], level);	// десен горен
	triangle([ 0, 1,-f], [-1, f, 0], [ 1, f, 0], level);	// десен долен
	triangle([ 0,-1, f], [-1,-f, 0], [ 1,-f, 0], level);	// ляв горен
	triangle([ 0,-1,-f], [ 1,-f, 0], [-1,-f, 0], level);	// ляв долен

	triangle([ 1, f, 0], [ f, 0, 1], [ f, 0,-1], level);	// предни и задни
	triangle([ 1,-f, 0], [ f, 0,-1], [ f, 0, 1], level);
	triangle([-1, f, 0], [-f, 0,-1], [-f, 0, 1], level);
	triangle([-1,-f, 0], [-f, 0, 1], [-f, 0,-1], level);

	triangle([ f, 0, 1], [ 0, 1, f], [ 0,-1, f], level);	// горни и долни
	triangle([-f, 0, 1], [ 0,-1, f], [ 0, 1, f], level);
	triangle([ f, 0,-1], [ 0,-1,-f], [ 0, 1,-f], level);
	triangle([-f, 0,-1], [ 0, 1,-f], [ 0,-1,-f], level);

	triangle([ 0, 1, f], [ f, 0, 1], [ 1, f, 0], level);	// горни ъглови 
	triangle([ 0, 1, f], [-1, f, 0], [-f, 0, 1], level);
	triangle([ 0,-1, f], [ 1,-f, 0], [ f, 0, 1], level); 
	triangle([ 0,-1, f], [-f, 0, 1], [-1,-f, 0], level);
	
	triangle([ 0, 1,-f], [ 1, f, 0], [ f, 0,-1], level);	// долни ъглови 
	triangle([ 0, 1,-f], [-f, 0,-1], [-1, f, 0], level);
	triangle([ 0,-1,-f], [ f, 0,-1], [ 1,-f, 0], level); 
	triangle([ 0,-1,-f], [-1,-f, 0], [-f, 0,-1], level);
	
	var buf = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,buf);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(data), gl.STATIC_DRAW);
	
	this.n = n; // запомняме броя триъгълници
	this.buf = buf;
}

// канонична геодезична сфера - метод за рисуване
CanonicalGeodesicSphere.prototype.draw = function()
{	
	gl.bindBuffer(gl.ARRAY_BUFFER,this.buf);
	// върхове
	gl.enableVertexAttribArray(aXYZ);
	gl.vertexAttribPointer(aXYZ,3,gl.FLOAT,false,3*FLOATS,0*FLOATS);
	// нормали
	gl.enableVertexAttribArray(aNormal);
	gl.vertexAttribPointer(aNormal,3,gl.FLOAT,false,3*FLOATS,0*FLOATS);
	// рисуваме n триъгълника
	gl.drawArrays(gl.TRIANGLES,0,3*this.n);
	// рисуваме n контура на триъгълниците
	//gl.vertexAttrib3fv(aColor,[0,0,0]);
	//for (var i=0; i<this.n; i++)
	//	gl.drawArrays(gl.LINE_LOOP,3*i,3);
}

// геодезична сфера - конструктор
GeodesicSphere = function(center,size)
{
	this.center = center;
	this.size = size;
	this.color = [1,0.75,0];
	this.n = GEODESIC_SIDES;
	this.offset = undefined;
	this.rot = undefined;
	// създаваме еднократно канонична геодезична сфера
	if (!canonicalGeodesicSphere[this.n])
		canonicalGeodesicSphere[this.n] = new CanonicalGeodesicSphere(this.n);
}

// геодезична сфера - рисуване
GeodesicSphere.prototype.draw = function()
{
	pushMatrix();
	gl.vertexAttrib3fv(aColor,this.color);
	translate(this.center);
	if (this.rot)
	{
		if (this.rot[0]) zRotate(this.rot[0]);	// хоризонтален ъгъл
		if (this.rot[1]) yRotate(this.rot[1]);	// вертикален ъгъл
		if (this.rot[2]) xRotate(this.rot[2]);	// вертикален ъгъл
		if (this.rot[3]) zRotate(this.rot[3]);	// осев ъгъл
	}
	scale([this.size,this.size,this.size]);
	if (this.offset) translate(this.offset); // и отместването
	useMatrix();
	canonicalGeodesicSphere[this.n].draw();
	popMatrix();
}


var ROTATIONAL_SIDES = 32;
var ROTATIONAL_LEVELS = 40;

// ротационен обект - конструктор
RotationalSolid = function(center,size,f)
{	
	// пресмята връх от ъгъл и височина
	function vertex(a,z)
	{
		var r = f(z);
		return [r*Math.cos(a),r*Math.sin(a),z];
	}

	// пресмята нормален вектор във връх с ъгъл a и височина z
	function normal(a,z)
	{
		var p = vertex(a,z);
		var u = vectorPoints(vertex(a+0.0001,z),p);
		var v = vectorPoints(vertex(a+0.0001,z+0.0001),p);
		return unitVector(vectorProduct(u,v));
	}
		
	// попълва в буфера връх и нормалният му вектор
	function dataPush(a,z)
	{	
		var p = vertex(a,z);
		var n = normal(a,z);
		data.push(p[0],p[1],p[2],n[0],n[1],n[2]);
	}
	
	var data = [];
	
	// генериране на хоризонтални ленти
	var dZ = 1/ROTATIONAL_LEVELS;
	for (var zi=0; zi<ROTATIONAL_LEVELS; zi++)
	{
		var a = 0, dA = 2*Math.PI/ROTATIONAL_SIDES;

		var z1 = zi*dZ;
		var z2 = (zi+1)*dZ;
		
		for (var ai=0; ai<=ROTATIONAL_SIDES; ai++)
		{
			dataPush(a,z1);
			dataPush(a,z2);
			a += dA;
		}
	}

	var buf = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,buf);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(data), gl.STATIC_DRAW);
	
	// запомняме брой ленти, брой триъгълници, буфера, центъра, размера, цвета
	this.l = ROTATIONAL_LEVELS;
	this.n = ROTATIONAL_SIDES*2+2;
	this.buf = buf;
	this.center = center;
	this.size = size;
	this.color = [[1,0.5,1],[0.9,0.4,0.8]];
	this.offset = undefined;
	this.rot = undefined;
}

// ротационен обект - метод за рисуване
RotationalSolid.prototype.draw = function()
{	
	pushMatrix();
	gl.vertexAttrib3fv(aColor,this.color);
	translate(this.center);
	if (this.rot)
	{
		if (this.rot[0]) zRotate(this.rot[0]);	// хоризонтален ъгъл
		if (this.rot[1]) yRotate(this.rot[1]);	// вертикален ъгъл
		if (this.rot[2]) xRotate(this.rot[2]);	// вертикален ъгъл
		if (this.rot[3]) zRotate(this.rot[3]);	// осев ъгъл
	}
	scale(this.size);
	if (this.offset) translate(this.offset); // и отместването
	useMatrix();

	// заради мащаба ползваме матрица за нормалите
	gl.uniform1i(uUseNormalMatrix,true);
	var nmat = calculateNormalMatrix(multiplyMatrix(glvmat,glmat));
	gl.uniformMatrix4fv(uNormalMatrix,false,nmat);

		gl.bindBuffer(gl.ARRAY_BUFFER,this.buf);
		// върхове
		gl.enableVertexAttribArray(aXYZ);
		gl.vertexAttribPointer(aXYZ,3,gl.FLOAT,false,6*FLOATS,0*FLOATS);
		// нормали
		gl.enableVertexAttribArray(aNormal);
		gl.vertexAttribPointer(aNormal,3,gl.FLOAT,false,6*FLOATS,3*FLOATS);
		// рисуваме лентите с редуване на цветовете
		for (var i=0; i<this.l; i++)
		{
			gl.vertexAttrib3fv(aColor,this.color[i%2]);
			gl.drawArrays(gl.TRIANGLE_STRIP,this.n*i,this.n);
		}
		
	gl.uniform1i(uUseNormalMatrix,false);

	popMatrix();
}


var TORUS_MAJOR_SIDES = 50;
var TORUS_MINOR_SIDES = 25;

// тор - конструктор
Torus = function(center,size,R,r)
{	
	// пресмята връх от два ъгъла
	function vertex(a,b)
	{
		var x = (R+r*Math.cos(b))*Math.cos(a);
		var y = (R+r*Math.cos(b))*Math.sin(a);
		var z = r*Math.sin(b);
		return [x,y,z];
	}

	// пресмята нормален вектор във връх с ъгъл a и височина z
	function normal(a,b)
	{
		var u = [-Math.cos(a)*Math.sin(b),-Math.sin(b)*Math.sin(a),Math.cos(b)];
		var v = [-Math.sin(a),Math.cos(a),0];
		return unitVector(vectorProduct(v,u));
	}
		
	// попълва в буфера връх и нормалният му вектор
	function dataPush(a,b,ai,bi)
	{	
		var p = vertex(a,b);
		var n = normal(a,b);
		data.push(p[0],p[1],p[2],n[0],n[1],n[2],ai,bi);
	}
	
	var data = [];
	
	var dA = 2*Math.PI/TORUS_MAJOR_SIDES;
	var dB = 2*Math.PI/TORUS_MINOR_SIDES;

	// генериране на ленти (по b)
	for (var bi=0; bi<TORUS_MINOR_SIDES; bi++)
	{
		var b1 = bi*dB;
		var b2 = (bi+1)*dB;
		
		// генериране на лента (по a)
		for (var ai=0; ai<=TORUS_MAJOR_SIDES; ai++)
		{
			var a = ai*dA;
			dataPush(a,b1,ai/TORUS_MAJOR_SIDES,bi/TORUS_MINOR_SIDES);
			dataPush(a,b2,ai/TORUS_MAJOR_SIDES,(bi+1)/TORUS_MINOR_SIDES);
		}
	}

	var buf = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,buf);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(data), gl.STATIC_DRAW);
	
	// запомняме брой ленти, брой триъгълници, буфера, центъра, размера, цвета
	this.l = TORUS_MINOR_SIDES;
	this.n = TORUS_MAJOR_SIDES*2+2;
	this.buf = buf;
	this.center = center;
	this.size = size;
	this.color = [0.5,0.75,1];
	this.offset = undefined;
	this.rot = undefined;
	this.texture = undefined; // неизвестна текстура
	this.texMatrix = new Float32Array([1,0,0,0,1,0,0,0,1]); // текстурна матрица
}

// тор - метод за рисуване
Torus.prototype.draw = function()
{	
	pushMatrix();
	gl.vertexAttrib3fv(aColor,this.color);
	translate(this.center);
	if (this.rot)
	{
		if (this.rot[0]) zRotate(this.rot[0]);	// хоризонтален ъгъл
		if (this.rot[1]) yRotate(this.rot[1]);	// вертикален ъгъл
		if (this.rot[2]) xRotate(this.rot[2]);	// вертикален ъгъл
		if (this.rot[3]) zRotate(this.rot[3]);	// осев ъгъл
	}
	scale([this.size,this.size,this.size]);
	if (this.offset) translate(this.offset); // и отместването
	useMatrix();

	gl.bindBuffer(gl.ARRAY_BUFFER,this.buf);
	// върхове
	gl.enableVertexAttribArray(aXYZ);
	gl.vertexAttribPointer(aXYZ,3,gl.FLOAT,false,8*FLOATS,0*FLOATS);
	// нормали
	gl.enableVertexAttribArray(aNormal);
	gl.vertexAttribPointer(aNormal,3,gl.FLOAT,false,8*FLOATS,3*FLOATS);
	// казваме къде са текстурите
	if (gl.isTexture(this.texture))
	{
		gl.bindTexture(gl.TEXTURE_2D,this.texture);
		gl.enableVertexAttribArray(aST);
		gl.vertexAttribPointer(aST,2,gl.FLOAT,false,8*FLOATS,6*FLOATS);
	}
	else
	{
		if (this.texture) gl.disableVertexAttribArray(aST);
	}
	if (this.texture) gl.uniformMatrix3fv(uTexMatrix,false,this.texMatrix);
	// рисуваме лентите
	for (var i=0; i<this.l; i++)
	{
		gl.drawArrays(gl.TRIANGLE_STRIP,this.n*i,this.n);
	}

	popMatrix();
}

// функция за създаване на текстурен обект от картинка
// функцията връща обекта веднага, но той става годна
// текстура по-късно, след зареждането на картинката
function loadTexture(url,post)
{
	var texture = gl.createTexture();
	var image = new Image();
	image.onload = function()
	{
		imageLoaded(texture,image);
		if (post) post(texture);
	};
	image.src = url;
	return texture;
}
	
// функция, която се извиква при зареждането на изображение
function imageLoaded(texture,image)
{
	gl.bindTexture(gl.TEXTURE_2D, texture);
	gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL,true);
	gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, image);
	gl.generateMipmap(gl.TEXTURE_2D);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER,gl.LINEAR);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_LINEAR);
	gl.bindTexture(gl.TEXTURE_2D, null);
}

// установява единична текстурна 3D матрица
function texIdentity()
{
	return new Float32Array([1,0,0,0,1,0,0,0,1]);
}

// транслира текстурна 3D матрица с 2D вектор 
function texTranslate(m,v)
{
	m[6] += m[0]*v[0]+m[3]*v[1];
	m[7] += m[1]*v[0]+m[4]*v[1];
}

// мащабира текстурна 3D матрица с 2D вектор 
function texScale(m,v)
{
	m[0] *= v[0];
	m[1] *= v[0];
	
	m[3] *= v[1];
	m[4] *= v[1];
}

// върти текстурна 3D матрица на ъгъл в градуси 
function texRotate(m,a)
{
	a = radians(a);
	var s = Math.sin(a);
	var c = Math.cos(a);
	
	a = m[0]*s+m[3]*c;
	m[0]=m[0]*c-m[3]*s;
	m[3]=a;
	
	a = m[1]*s+m[4]*c;
	m[1]=m[1]*c-m[4]*s;
	m[4]=a;
}


// функция за удобство - връща X координата на миши събитие
function getX(event)
{
	return event.clientX-gl.canvas.offsetLeft-gl.canvas.offsetWidth/2;
}

// функция за удобство - връща X координата на миши събитие
function getY(event)
{
	return -event.clientY+gl.canvas.offsetTop+gl.canvas.offsetHeight/2;
}

var time = now();
function now() { return (new Date()).getTime()/1000; }

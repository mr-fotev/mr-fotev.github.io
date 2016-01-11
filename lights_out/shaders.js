var vShader =
	'uniform mat4 uProjectionMatrix;'+
	'uniform mat4 uViewMatrix;'+
	'uniform mat4 uModelMatrix;'+
	'uniform mat4 uNormalMatrix;'+
	'uniform bool uUseNormalMatrix;'+
	''+
	'uniform bool uSelectMode;'+
	'uniform vec3 uSelectColor;'+
	''+
	'uniform vec3 uAmbientColor;'+
	'uniform vec3 uDiffuseColor;'+
	''+
	'uniform vec3 uLightDir;'+
	''+
	'attribute vec3 aXYZ;'+
	'attribute vec3 aColor;'+
	'attribute vec3 aNormal;'+
	''+
	'varying vec3 vColor;'+
	''+
	'void main ()'+
	'{'+
	'	mat4 mvMatrix = uViewMatrix * uModelMatrix;'+
	'	gl_Position = uProjectionMatrix * mvMatrix * vec4(aXYZ,1);'+
	'	if (uSelectMode)'+
	'	{'+
	'		vColor = uSelectColor;'+
	'	}'+
	'	else'+
	'	{'+
	'		mat4 nMatrix = uUseNormalMatrix?uNormalMatrix:mvMatrix;'+
	''+
	'		vColor = uAmbientColor*aColor;'+
	''+
	'		vec3 light = normalize(-uLightDir);'+
	'		vec3 normal = vec3(normalize(nMatrix*vec4(aNormal,0)));'+
	'		vColor += aColor*uDiffuseColor*abs(dot(normal,light));'+
	'	}'+
	'}';
	
var fShader =
	'precision mediump float;'+
	''+
	'varying vec3 vColor;'+
	'void main( )'+
	'{'+
	'	gl_FragColor = vec4(vColor,1.0);'+
	'}';

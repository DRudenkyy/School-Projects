//
// Alternate vertex shader for the transformation assignment
//
// Created by Warren R. Carithers 2016/04/22.
//
// Contributor:  Dmytro Rudenkyy
//

#version 120

// attribute:  vertex position
attribute vec4 vPosition;

uniform int projectionType;

//Projection settings: regardless of the projection type being used, 
//the boundaries on the view volume will be
float left = -1.0,
	right = 1.0,
	top = 1.0,
	bottom = -1.0,
	near = 0.9,
	far = 4.5;
	
//parameters
uniform vec3 ey; //camera/view settings
uniform vec3 la;
uniform vec3 up;
/*
//used to apply alternate camera parameters from main
Tuple alt_eye =    { 0.0f, 1.3f, -0.5f };
Tuple alt_lookat = { 0.0f, -0.4f, -1.0f };
Tuple alt_up =     { 0.0f, 1.0f, 0.0f };
*/
uniform vec3 sc; //model transformation
uniform vec3 ro;	//in degrees
uniform vec3 tr;
/*
//used to apply transformation values from main
Tuple alt_scale =  { 1.0f, 2.0f, 1.0f };
Tuple alt_rotate = { 0.0f, 10.0f, 335.0f };
Tuple alt_xlate =  { -0.2f, 0.2f, 0.0f };
*/

//https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)
//example of a function in GLSL
//void MyFunction(in float inputValue, out int outputValue, inout float inAndOutValue);
//Functions in GLSL use a calling convention called "value-return."
/*A parameter declared as out will not have its value initialized by the caller. 
 * The function will modify the parameter, and after the function's execution is 
 * complete, the value of the parameter will be copied out into the variable 
 * that the user specified when calling the function. Note that the initial 
 * value of the parameter at the start of the function being called is undefined,
 *  just as if one had simply created a local variable.
*/

//set the projection matrix to frustum 
void setFrustum(out mat4 projection) {
	projection = mat4((2 * near) / (right - left), 0, 0, 0,
		0, (2 * near) / (top - bottom), 0, 0,
		(right + left) / (right - left), (top + bottom) / (top - bottom), -(far + near) / (far - near), -1,
		0, 0, (-2 * far * near) / (far - near), 0);
}

//set the projection matrix to ortho
void setOrtho(out mat4 projection) {
	
	projection = mat4(2.0 / (right - left), 0, 0, 0,
		0, 2.0 / (top - bottom), 0, 0,
		0, 0, (-2.0/(far - near)), 0,
		-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1);
}

//set the view/camera matrix
void setView(out mat4 view) {
	//GLSL functions I will be using (from lighthouse3d.com)
	//normalize — calculates the unit vector in the same direction as 
	//	the original vector
	//cross - The cross product takes two vectors and returns a 
	//	perpendicular vector to the plane defined by the two vectors
	//dot - The dot product, aka inner product, takes two vectors and 
	//	returns a scalar value. It is an easy way to compute the cosine 
	//between two vectors.
	
	//n = normalize( eyepoint - lookat )
	vec3 n = normalize(ey - la);
	
	//u = normalize( normalize( up ) x n )
	vec3 u = normalize(cross(normalize(up), n));
	
	//v = normalize( n x u ) 
	vec3 v = normalize(cross(n, u));
	
	view = mat4(u.x, v.x, n.x, 0,
				u.y, v.y, n.y, 0,
				u.z, v.z, n.z, 0,
				(-1 * (dot(u, ey))), (-1 * (dot(v, ey))), (-1 * (dot(n, ey))), 1);
}

//set translation matrix
void setTranslate(out mat4 t) {
	t = mat4(1, 0, 0, 0,
			 0, 1, 0, 0,
			 0, 0, 1, 0,
			 tr.x, tr.y, tr.z, 1);
	
}

//set scaling matrix
void setScale(out mat4 s) {
	s = mat4(sc.x, 0, 0, 0,
			 0, sc.y, 0, 0,
			 0, 0, sc.z, 0,
			 0, 0, 0, 1);
	
}

//set rotation matrix
void setRotate(out mat4 r) {
	
	//convert rotation vector to radian floats
	float rx = radians(ro.x);	
	float ry = radians(ro.y);
	float rz = radians(ro.z);

	//breaking down rotation into three seperate matrices
	
	//from Z towards Y
	mat4 aboutX = mat4(1, 0, 0, 0,
						0, cos(rx), sin(rx), 0,
						0, -sin(rx), cos(rx), 0,
						0, 0, 0, 1);
			 
	//from X towards Z
	mat4 aboutY = mat4(cos(ry), 0, -sin(ry), 0,
						0, 1, 0, 0,
						sin(ry), 0, cos(ry), 0,
						0, 0, 0, 1);
	
	//from X towards Y
	mat4 aboutZ = mat4(cos(rz), sin(rz), 0, 0,
						-sin(rz), cos(rz), 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1);
			 
	//multiply all matrices to get final solution
	r = aboutZ * aboutY * aboutX;
	
}

//The vertex shader is responsible for at least writing a variable: gl_Position, 
//usually transforming the vertex with the modelview and projection matrices.
void main()
{
	mat4 projection, view, s, r, t, finalTranslation;

	//set the projection matrix
	if (projectionType == 1) {
		setFrustum(projection);
	}
	else {
		setOrtho(projection);
	}
	
	setView(view);
	
	setScale(s);
	setRotate(r);
	setTranslate(t);
	
	finalTranslation = s * r * t;
	
    gl_Position = projection * view * s * t * vPosition;
    //uncomment for rotation matrix included
    //gl_Position = projection * view * finalTranslation * vPosition;
}

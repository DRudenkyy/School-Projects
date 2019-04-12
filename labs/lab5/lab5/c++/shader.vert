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
uniform vec3 ro;
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
	projection = mat4(2 / (right - left), 0, 0, 0,
		0, 2 / (top - bottom), 0, 0,
		0, 0, -(far + near) / (far - near), -1,
		(right + left) / (right - left), (top + bottom) / (top - bottom), (-2 * far * near) / (far - near), 0);
}

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
	
	view = mat4(0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 1);
}

//The vertex shader is responsible for at least writing a variable: gl_Position, 
//usually transforming the vertex with the modelview and projection matrices.
void main()
{
	mat4 projection, view;

	//set the projection matrix
	if (projectionType == 1) {
		setFrustum(projection);
	}
	else {
		setOrtho(projection);
	}
	
	setView(view);
	
    gl_Position =  projection * vPosition;
}

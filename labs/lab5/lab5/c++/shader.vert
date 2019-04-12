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

// Projection settings: regardless of the projection type being used, 
// the boundaries on the view volume will be
float left = -1.0,
	right = 1.0,
	top = 1.0,
	bottom = -1.0,
	near = 0.9,
	far = 4.5;
	
//parameters
uniform vec3 ey; // view
uniform vec3 la;
uniform vec3 up;
/*
//used to apply alternate camera parameters from main
Tuple alt_eye =    { 0.0f, 1.3f, -0.5f };
Tuple alt_lookat = { 0.0f, -0.4f, -1.0f };
Tuple alt_up =     { 0.0f, 1.0f, 0.0f };
*/
uniform vec3 sc; // transformation
uniform vec3 ro;
uniform vec3 tr;
/*
//used to apply transformation values from main
Tuple alt_scale =  { 1.0f, 2.0f, 1.0f };
Tuple alt_rotate = { 0.0f, 10.0f, 335.0f };
Tuple alt_xlate =  { -0.2f, 0.2f, 0.0f };
*/

void main()
{
    // By default, no transformations are performed.
    gl_Position =  vPosition;
}

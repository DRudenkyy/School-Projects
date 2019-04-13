///
//  Viewing
//
//  Created by Warren R. Carithers 2016/11/11.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Simple class for setting up the viewing and projection transforms
//  for the Transformation Assignment.
//
//  This code can be compiled as either C or C++.
//
//  Contributor:  Dmytro Rudenkyy
///

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "Viewing.h"

///
// This function sets up the view and projection parameters for the
// desired projection of the scene. See the assignment description for
// the values for the projection parameters.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param mode - The desired projection mode (Frustum or Ortho)
///
void setUpProjection( GLuint program, ViewMode mode )
{
	int setMode = -1;
	switch(mode)
	{
		case Frustum:
			setMode = 1;
			break;
		case Ortho:
			setMode = 0;
			break;
		default:
			std::cerr << "Invalid viewmode";
			break;
	}
    glUniform1i(glGetUniformLocation(program, "projectionType"), setMode);
}

///
// This function clears any model transformations, setting the values
// to the defaults: no scaling (scale factor of 1), no rotation (degree
// of rotation = 0), and no translation (0 translation in each direction).
//
// You will need to write this function, and maintain all of the values
// which must be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void clearTransforms( GLuint program )
{
    glUniform3f(glGetUniformLocation(program, "sc"), 1.0, 1.0, 1.0);
	glUniform3f(glGetUniformLocation(program, "ro"), 0.0, 0.0, 0.0);
    glUniform3f(glGetUniformLocation(program, "tr"), 0.0, 0.0, 0.0);
}

///
// This function sets up the transformation parameters for the vertices
// of the teapot.  The order of application is specified in the driver
// program.
//
// You will need to write this function, and maintain all of the values
// which must be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param scale  - scale factors for each axis
// @param rotate - rotation angles around the three axes, in degrees
// @param xlate  - amount of translation along each axis
///
void setUpTransforms( GLuint program, Tuple scale, Tuple rotate, Tuple xlate )
{
    glUniform3f(glGetUniformLocation(program, "sc"), scale.x, scale.y, scale.z);
	glUniform3f(glGetUniformLocation(program, "ro"), rotate.x, rotate.y, rotate.z);
    glUniform3f(glGetUniformLocation(program, "tr"), xlate.x, xlate.y, xlate.z);
}

///
// This function clears any changes made to camera parameters, setting the
// values to the defaults: eye (0.0,0.0,0.0), lookat (0,0,0.0,-1.0),
// and up vector (0.0,1.0,0.0).
//
// You will need to write this function, and maintain all of the values
// which must be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void clearCamera( GLuint program )
{
    glUniform3f(glGetUniformLocation(program, "ey"), 0.0, 0.0, 0.0);
	glUniform3f(glGetUniformLocation(program, "la"), 0.0, 0.0, -1.0);
    glUniform3f(glGetUniformLocation(program, "up"), 0.0, 1.0, 0.0);
}

///
// This function sets up the camera parameters controlling the viewing
// transformation.
//
// You will need to write this function, and maintain all of the values
// which must be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param eye    - camera location
// @param lookat - lookat point
// @param up     - the up vector
///
void setUpCamera( GLuint program, Tuple eye, Tuple lookat, Tuple up )
{
    glUniform3f(glGetUniformLocation(program, "ey"), eye.x, eye.y, eye.z);
	glUniform3f(glGetUniformLocation(program, "la"), lookat.x, lookat.y, lookat.z);
    glUniform3f(glGetUniformLocation(program, "up"), up.x, up.y, up.z);
}

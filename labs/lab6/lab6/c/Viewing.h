///
//  Viewing.h
//
//  Created by Warren R. Carithers on 2016/11/11.
//  Based on a version created by Joe Geigel on 1/23/13.
//  Copyright 2016 Rochester Institute of Technology. All rights reserved.
//  
//  Code for setting up all the parameters necessary to implement
//  model, camera, and projection transformations.
//  
//  This code can be compiled as either C or C++.
///

#ifndef _VIEWING_H_
#define _VIEWING_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "Tuple.h"

///
// This function sets up the view and projection parameter for a frustum
// projection of the scene.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpProjection( GLuint program );

///
// This function clears any transformations, setting the values to the
// defaults: scale by 4 in Y, rotate by 50 in Y and 90 in Z, and
// translate by 1 in X and -1 in Z.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void clearTransforms( GLuint program );

///
// This function sets up the transformation parameters for the vertices
// of the teapot.  The order of application is specified in the driver
// program.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param scale  - scale factors for each axis
// @param rotate - rotation angles around the three axes, in degrees
// @param xlate  - amount of translation along each axis
///
void setUpTransforms( GLuint program, Tuple scale, Tuple rotate, Tuple xlate );

///
// This function clears any changes made to camera parameters, setting the
// values to the defaults: eyepoint (0.0,3.0,3.0), lookat (1,0,0.0,0.0),
// and up vector (0.0,1.0,0.0).
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void clearCamera( GLuint program );

///
// This function sets up the camera parameters controlling the viewing
// transformation.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param eye    - camera location
// @param lookat - lookat point
// @param up     - the up vector
///
void setUpCamera( GLuint program, Tuple eye, Tuple lookat, Tuple up );

#endif

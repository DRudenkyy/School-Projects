///
// Sphere.h
//
// Prototypes for shape-drawing functions.
//
// This file should not be modified by students.
///

#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Shapes.h"
#include "Canvas.h"
#include "Buffers.h"

///
// makeSphere
//
// Invoked whenever a sphere must be created
//
// @param C - Canvas being used
///
void makeSphere( Canvas *C );

///
// drawSphere
//
// Invoked whenever the sphere must be redrawn
//
// @param pshader - shader program for Phong shading
// @param tshader - shader program for texture mapping
// @param bset    - the BufferSet containing the object's data
///
void drawSphere( GLuint pshader, GLuint tshader, BufferSet *bset );

#endif

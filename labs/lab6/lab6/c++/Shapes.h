///
// Shapes.h
//
// Prototypes for shape-drawing functions.
//
// This file should not be modified by students.
///

#ifndef _SHAPES_H_
#define _SHAPES_H_

#include "Canvas.h"
#include "Buffers.h"

// Macros for object and shading selection
#define OBJ_QUAD	0
#define OBJ_TEAPOT	1
#define OBJ_SPHERE	2

///
// makeTeapot
//
// Invoked whenever a teapot must be created
//
// @param C - Canvas being used
///
void makeTeapot( Canvas &C );

///
// makeQuad
//
// Invoked whenever a quad must be created
//
// @param C - Canvas being used
///
void makeQuad( Canvas &C );

///
// drawShape
//
// Invoked whenever an object must be redrawn
//
// @param shader - shader program to use
// @param obj    - which object is being drawn
// @param bset   - the BufferSet containing the object's data
///
void drawShape( GLuint shader, int obj, BufferSet &bset );

#endif

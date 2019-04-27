///
//  Canvas.h
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on a C++ version created by Joe Geigel.
//  Updates 2018/11/29 by wrc.
//  Copyright 2016 Rochester Institute of Technology. All rights reserved.
//
//  Prototypes for routines for manipulating a simple canvas
//  holding point information along with color data.
//
//  This file should not be modified by students.
///

#ifndef _CANVAS_H_
#define _CANVAS_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "FloatVector.h"
#include "Vertex.h"
#include "Normal.h"
#include "UVcoord.h"

///
// Simple canvas structure that allows for pixel-by-pixel rendering.
///

typedef struct st_Canvas {

    ///
    // canvas size information
    ///
    int width;
    int height;

    ///
    // point-related data
    ///

    // vertex locations
    FloatVector points;
    float *pointArray;

    // associated normal vectors
    FloatVector normals;
    float *normalArray;

    // associated (u,v) coordinates
    FloatVector uv;
    float *uvArray;

    // associated color data
    FloatVector colors;
    float *colorArray;

    // element count and connectivity data
    int numElements;
    GLuint *elemArray;

    ///
    // current drawing color
    ///
    GLfloat currentColor[3]; // RGB only - A is fixed

} Canvas;


///
// Constructor
//
// @param w width of canvas
// @param h height of canvas
///
Canvas *canvasCreate( int w, int h );

///
// Destructor
//
// @param C canvas to destroy
///
void canvasDestroy( Canvas *C );

///
// clears the canvas
//
// @param C The Canvas to use
///
void canvasClear( Canvas *C );

///
// adds a triangle to the current shape
//
// @param p0 first triangle vertex
// @param p1 second triangle vertex
// @param p2 final triangle vertex
// @param C The Canvas to be used
///
void canvasAddTriangle( Vertex p0, Vertex p1, Vertex p2, Canvas *C );

///
// adds a triangle to the current shape, along with (u,v) data
//
// @param p0 first vertex
// @param uv0 first vertex (u,v) data
// @param p1 second vertex
// @param uv1 second vertex (u,v) data
// @param p2 final vertex
// @param uv2 final vertex (u,v) data
// @param C The Canvas to be used
///
void canvasAddTriangleWithUV( Vertex p0, UVcoord uv0,
    Vertex p1, UVcoord uv1, Vertex p2, UVcoord uv2, Canvas *C );

///
// adds a triangle to the current shape, along with normal data
//
// @param p0 first vertex
// @param n0 first vertex normal data
// @param p1 second vertex
// @param n1 second vertex normal data
// @param p2 final vertex
// @param n2 final vertex normal data
// @param C The Canvas to be used
///
void canvasAddTriangleWithNorms( Vertex p0, Normal n0,
    Vertex p1, Normal n1, Vertex p2, Normal n2, Canvas *C );

///
// Sets the current color
//
// @param r The red component of the new color (between 0-1)
// @param g The green component of the new color (between 0-1)
// @param b The blue component of the new color (between 0-1);
// @param C The Canvas to use
///
void canvasSetColor( float r, float g, float b, Canvas *C );

///
// writes a pixel using the current color
//
// @param x The x coord of the pixel to be set
// @param y The y coord of the pixel to be set
// @param C The Canvas to use
///
void canvasSetPixel( int x, int y, Canvas *C );

///
// retrieve the array of element data from this Canvas
//
// @param C The Canvas to be used
///
GLuint *canvasGetElements( Canvas *C );

///
// retrieve the array of vertex data from this Canvas
//
// @param C The Canvas to be used
///
float *canvasGetVertices( Canvas *C );

///
// retrieve the array of normal data from this Canvas
//
// @param C The Canvas to be used
///
float *canvasGetNormals( Canvas *C );

///
// retrieve the array of (u,v) data from this Canvas
//
// @param C The Canvas to be used
///
float *canvasGetUV( Canvas *C );

///
// retrieve the array of color data from this Canvas
//
// @param C The Canvas to be used
///
float *canvasGetColors( Canvas *C );

///
// retrieve the vertex count from this Canvas
//
// @param C The Canvas to be used
///
int canvasNumVertices( Canvas *C );

#endif

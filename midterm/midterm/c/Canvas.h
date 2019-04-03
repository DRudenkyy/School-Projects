//
//  Canvas.h
//
//  Created by Warren R. Carithers 2016/09/23.
//  Based on a C++ version created by Joe Geigel.
//  Copyright 2016 Rochester Institute of Technology. All rights reserved.
//
//  Prototypes for routines for manipulating a simple canvas
//  holding point information along with color data.
//
//  This file should not be modified by students.
//

#ifndef _CANVAS_H_
#define _CANVAS_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <stdbool.h>

#include "FloatVector.h"

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
    FloatVector points;
    float *pointArray;
    int numElements;
    GLuint *elemArray;
    float depth;

    ///
    // color-related data
    ///
    FloatVector colors;
    float *colorArray;
    GLfloat currentColor[3]; // RGB only - A is fixed

    ///
    // outline vs. polygon drawing variables
    // (used by the driver program to determine how to draw things)
    ///
    bool drawingOutlines;
    int countNum;
    const int countNumLimit;
    int *outlineCounts;

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
// set the pixel Z coordinate
//
// @param d The depth to use
// @param C The Canvas to use
//
// @return  The old depth value
///
float canvasSetDepth( float d, Canvas *C );

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
// Draw the outline of a polygon.
//
// Use this drawOutline() method only if you were NOT able to
// create a working drawPolygon() Rasterizer routine of your own.
// This method will only draw the outline of the polygon.
//
// @param n The number of vertices in the polygon
// @param x The x coordinates of the vertices
// @param y the y coordinates of the vertices
// @param C The Canvas to use
///
void canvasDrawOutline( int n, const int x[], const int y[], Canvas *C );

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

///
// returns outline status of this object
//
// @param C The Canvas to be used
///
bool canvasIsOutline( Canvas *C );

#endif
//
//  Canvas.c
//
//  Created by Warren R. Carithers 2016/09/30.
//  Based on a C++ version created by Joe Geigel.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Routines for adding points to create a new mesh.
//
//  This file should not be modified by students.
//

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "Canvas.h"
#include "FloatVector.h"

///
// Constructor
//
// @param w width of canvas
// @param h height of canvas
///
Canvas *canvasCreate( int w, int h ) {
    Canvas *C = calloc( 1, sizeof(Canvas) );
    if( C != NULL ) {
	// This is how to initialize a const int field when the
	// memory is dynamically allocated.  The partial initializer
	// list puts 100 into countNumLimit, and leaves 0 in all the
	// other fields.
	Canvas init = { .countNumLimit = 100 };
	// next, we copy it into the "real" Canvas
	memcpy( C, &init, sizeof(Canvas) );
	// and then set the size fields
        C->width = w;
        C->height = h;
	C->depth = -1.0f;
        // all other fields contain zeroes
    }
    return( C );
}


///
// Destructor
//
// @param C canvas to destroy
///
void canvasDestroy( Canvas *C ) {
    if( !C )
        return;
    free( C );
}


///
// clear the canvas
//
// @param C The Canvas to use
///
void canvasClear( Canvas *C )
{
    if( C->pointArray ) {
        free( C->pointArray );
        C->pointArray = 0;
    }
    if( C->elemArray ) {
        free( C->elemArray );
        C->elemArray = 0;
    }
    if( C->colorArray ) {
        free( C->colorArray );
        C->colorArray = 0;
    }
    if( C->outlineCounts ) {
        free( C->outlineCounts );
        C->outlineCounts = 0;
    }
    C->outlineCounts = (int *) calloc( C->countNumLimit, sizeof(int) );
    fvClear( &(C->points) );
    fvClear( &(C->colors) );
    C->numElements = 0;
    C->currentColor[0] = 0.0;
    C->currentColor[1] = 0.0;
    C->currentColor[2] = 0.0;
    C->drawingOutlines = false;
    C->countNum = 0;
}


///
// set the pixel Z coordinate
//
// @param d The depth to use
// @param C The Canvas to use
//
// @return  The old depth value
///
float canvasSetDepth( float d, Canvas *C )
{
    float r = C->depth;

    C->depth = d;
    return( r );
}


///
// change the current drawing color
//
// @param r The red component of the new color (between 0-1)
// @param g The green component of the new color (between 0-1)
// @param b The blue component of the new color (between 0-1);
// @param C The Canvas to use
///
void canvasSetColor( float r, float g, float b, Canvas *C )
{
    C->currentColor[0] = r;
    C->currentColor[1] = g;
    C->currentColor[2] = b;
}


///
// set a pixel to the current drawing color
//
// @param x The x coord of the pixel to be set
// @param y The y coord of the pixel to be set
// @param C The Canvas to use
///
void canvasSetPixel( int x0, int y0, Canvas *C )
{
    fvPushBack( &(C->points), (float) x0 );
    fvPushBack( &(C->points), (float) y0 );
    fvPushBack( &(C->points), C->depth );  // fixed Z depth
    fvPushBack( &(C->points), 1.0 );

    fvPushBack( &(C->colors), C->currentColor[0] );
    fvPushBack( &(C->colors), C->currentColor[1] );
    fvPushBack( &(C->colors), C->currentColor[2] );
    fvPushBack( &(C->colors), 1.0 );   // alpha channel

    C->numElements += 1;
}


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
/// 

void canvasDrawOutline( int n, const int x[], const int y[], Canvas *C )
{
    if( C->countNum >= C->countNumLimit ) {
        fprintf( stderr, "too many outlines to draw!\n" );
        return;
    }

    C->drawingOutlines = true;
    C->outlineCounts[(C->countNum)++] = n;

    for( int i = 0; i < n; i++ )
        canvasSetPixel( x[i], y[i], C );
}


///
// gets the array of vertices for the current shape
//
// @param C The Canvas to use
///
float *canvasGetVertices( Canvas *C )
{
    int i;

    // delete the old point array if we have one
    if( C->pointArray ) {
        free( C->pointArray );
    }

    // create and fill a new point array
    C->pointArray = (float *) malloc(
        fvSize(&(C->points)) * sizeof(float) );
    if( C->pointArray == 0 ) {
    	perror( "point allocation failure" );
	exit( 1 );
    }
    for( i = 0; i < fvSize(&(C->points)); i++ ) {
        C->pointArray[i] = C->points.vec[i];
    }

    return C->pointArray;
}


///
// gets the array of elements for the current shape
//
// @param C The Canvas to use
///
GLuint *canvasGetElements( Canvas *C )
{
    int i;

    // delete the old element array if we have one
    if( C->elemArray ) {
        free( C->elemArray );
    }

    // create and fill a new element array
    C->elemArray = (GLuint *) malloc(
        fvSize(&(C->points)) * sizeof(GLuint) );
    if( C->elemArray == 0 ) {
    	perror( "element allocation failure" );
	exit( 1 );
    }
    for( i = 0; i < fvSize(&(C->points)); i++ ) {
        C->elemArray[i] = i;
    }

    return C->elemArray;
}


///
// gets the array of colors for the current shape
//
// @param C The Canvas to use
///
float *canvasGetColors( Canvas *C )
{
    int i;

    // delete the old color array if we have one
    if( C->colorArray ) {
        free( C->colorArray );
    }

    // create and fill a new color array
    C->colorArray = (float *) malloc(
        fvSize(&(C->colors)) * sizeof(float) );
    if( C->colorArray == 0 ) {
    	perror( "color allocation failure" );
	exit( 1 );
    }
    for( i = 0; i < fvSize(&(C->colors)); i++ ) {
        C->colorArray[i] = C->colors.vec[i];
    }

    return C->colorArray;
}


///
// returns number of vertices in current shape
//
// @param C The Canvas to use
///
int canvasNumVertices( Canvas *C )
{
    return C->numElements;
}


///
// returns outline status of this object
///
bool canvasIsOutline( Canvas *C )
{
    return C->drawingOutlines;
}

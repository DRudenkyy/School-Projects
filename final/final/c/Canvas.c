///
//  Canvas.c
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on a C++ version created by Joe Geigel.
//  Updates 2018/11/29 by wrc.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Routines for adding points to create a new mesh.
//
//  This file should not be modified by students.
///

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
        C->width = w;
        C->height = h;
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
    if( C->normalArray ) {
        free( C->normalArray );
        C->normalArray = 0;
    }
    if( C->uvArray ) {
        free( C->uvArray );
        C->uvArray = 0;
    }
    if( C->elemArray ) {
        free( C->elemArray );
        C->elemArray = 0;
    }
    if( C->colorArray ) {
        free( C->colorArray );
        C->colorArray = 0;
    }
    fvClear( &(C->points) );
    fvClear( &(C->normals) );
    fvClear( &(C->uv) );
    fvClear( &(C->colors) );
    C->numElements = 0;
    C->currentColor[0] = 0.0;
    C->currentColor[1] = 0.0;
    C->currentColor[2] = 0.0;
}

///
// adds a triangle to the current shape
//
// @param p0 first triangle vertex
// @param p1 second triangle vertex
// @param p2 final triangle vertex
// @param C The Canvas to be used
///
void canvasAddTriangle( Vertex p0, Vertex p1, Vertex p2, Canvas *C )
{
    fvPushBack( &(C->points), p0.x );
    fvPushBack( &(C->points), p0.y );
    fvPushBack( &(C->points), p0.z );
    fvPushBack( &(C->points), 1.0f );

    fvPushBack( &(C->points), p1.x );
    fvPushBack( &(C->points), p1.y );
    fvPushBack( &(C->points), p1.z );
    fvPushBack( &(C->points), 1.0f );

    fvPushBack( &(C->points), p2.x );
    fvPushBack( &(C->points), p2.y );
    fvPushBack( &(C->points), p2.z );
    fvPushBack( &(C->points), 1.0f );

    C->numElements += 3;  // three vertices per triangle
}

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
    Vertex p1, UVcoord uv1, Vertex p2, UVcoord uv2, Canvas *C )
{
    // calculate the normal
    float ux = p1.x - p0.x;
    float uy = p1.y - p0.y;
    float uz = p1.z - p0.z;

    float vx = p2.x - p0.x;
    float vy = p2.y - p0.y;
    float vz = p2.z - p0.z;

    Normal nn = { (uy * vz) - (uz * vy),
                  (uz * vx) - (ux * vz),
                  (ux * vy) - (uy * vx) };

    // Attach the normal to all 3 vertices
    canvasAddTriangleWithNorms( p0, nn, p1, nn, p2, nn, C );

    // Attach the texture coordinates
    fvPushBack( &(C->uv), uv0.u );
    fvPushBack( &(C->uv), uv0.v );
    fvPushBack( &(C->uv), uv1.u );
    fvPushBack( &(C->uv), uv1.v );
    fvPushBack( &(C->uv), uv2.u );
    fvPushBack( &(C->uv), uv2.v );
}

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
    Vertex p1, Normal n1, Vertex p2, Normal n2, Canvas *C )
{
    fvPushBack( &(C->points), p0.x );
    fvPushBack( &(C->points), p0.y );
    fvPushBack( &(C->points), p0.z );
    fvPushBack( &(C->points), 1.0f );

    fvPushBack( &(C->normals), n0.x );
    fvPushBack( &(C->normals), n0.y );
    fvPushBack( &(C->normals), n0.z );

    fvPushBack( &(C->points), p1.x );
    fvPushBack( &(C->points), p1.y );
    fvPushBack( &(C->points), p1.z );
    fvPushBack( &(C->points), 1.0f );

    fvPushBack( &(C->normals), n1.x );
    fvPushBack( &(C->normals), n1.y );
    fvPushBack( &(C->normals), n1.z );

    fvPushBack( &(C->points), p2.x );
    fvPushBack( &(C->points), p2.y );
    fvPushBack( &(C->points), p2.z );
    fvPushBack( &(C->points), 1.0f );

    fvPushBack( &(C->normals), n2.x );
    fvPushBack( &(C->normals), n2.y );
    fvPushBack( &(C->normals), n2.z );

    C->numElements += 3;  // three vertices per triangle

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
    fvPushBack( &(C->points), -1.0 );  // fixed Z depth
    fvPushBack( &(C->points), 1.0 );

    fvPushBack( &(C->colors), C->currentColor[0] );
    fvPushBack( &(C->colors), C->currentColor[1] );
    fvPushBack( &(C->colors), C->currentColor[2] );
    fvPushBack( &(C->colors), 1.0 );   // alpha channel

    C->numElements += 1;
}

///
// gets the array of vertices for the current shape
//
// @param C The Canvas to use
///
float *canvasGetVertices( Canvas *C )
{
    // delete the old point array if we have one
    if( C->pointArray ) {
        free( C->pointArray );
        C->pointArray = 0;
    }

    int n = fvSize( &(C->points) );

    if( n > 0 ) {
        // create and fill a new point array
        C->pointArray = (float *) malloc( n * sizeof(float) );
        if( C->pointArray == 0 ) {
    	    perror( "point allocation failure" );
	    exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            C->pointArray[i] = C->points.vec[i];
        }
    }

    return C->pointArray;
}

///
// gets the array of normals for the current shape
//
// @param C The Canvas to use
///
float *canvasGetNormals( Canvas *C )
{
    // delete the old normal array if we have one
    if( C->normalArray ) {
        free( C->normalArray );
        C->normalArray = 0;
    }

    int n = fvSize( &(C->normals) );

    if( n > 0 ) {
        // create and fill a new normal array
        C->normalArray = (float *) malloc( n * sizeof(float) );
        if( C->normalArray == 0 ) {
    	    perror( "normal allocation failure" );
	    exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            C->normalArray[i] = C->normals.vec[i];
        }
    }

    return C->normalArray;
}

///
// gets the array of texture coordinates for the current shape
//
// @param C The Canvas to use
///
float *canvasGetUV( Canvas *C )
{
    // delete the old texture coordinate array if we have one
    if( C->uvArray ) {
        free( C->uvArray );
        C->uvArray = 0;
    }

    int n = fvSize( &(C->uv) );

    if( n > 0 ) {
        // create and fill a new texture coordinate array
        C->uvArray = (float *) malloc( n * sizeof(float) );
        if( C->uvArray == 0 ) {
    	    perror( "uv allocation failure" );
	    exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            C->uvArray[i] = C->uv.vec[i];
        }
    }

    return C->uvArray;
}


///
// gets the array of elements for the current shape
//
// @param C The Canvas to use
///
GLuint *canvasGetElements( Canvas *C )
{
    // delete the old element array if we have one
    if( C->elemArray ) {
        free( C->elemArray );
        C->elemArray = 0;
    }

    int n = C->numElements;

    if( n > 0 ) {
        // create and fill a new element array
        C->elemArray = (GLuint *) malloc( n * sizeof(GLuint) );
        if( C->elemArray == 0 ) {
    	    perror( "element allocation failure" );
	    exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            C->elemArray[i] = i;
        }
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
    // delete the old color array if we have one
    if( C->colorArray ) {
        free( C->colorArray );
        C->colorArray = 0;
    }

    int n = fvSize( &(C->colors) );

    if( n > 0 ) {
        // create and fill a new color array
        C->colorArray = (float *) malloc( n * sizeof(float) );
        if( C->colorArray == 0 ) {
    	    perror( "color allocation failure" );
	    exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            C->colorArray[i] = C->colors.vec[i];
        }
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

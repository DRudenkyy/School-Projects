//
//  clipMain.c
//
//  Main program for polygon clip assignment
//
//  Created by Warren R. Carithers on 02/27/14.
//  Based on a C++ version created by Joe Geigel on 11/30/11.
//  Copyright 2011 Rochester Institute of Technology. All rights reserved.
//
//  This file should not be modified by students.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "ShaderSetup.h"
#include "Vertex.h"
#include "Buffers.h"
#include "Canvas.h"
#include "Clipper.h"

///
// Object information
///

// our clipping regions (ll and ur corners)

Vertex clip1[2] = { {  10, 110 }, {  50, 150 } };
Vertex clip2[2] = { {  30,  30 }, {  70,  80 } };
Vertex clip3[2] = { {  90,  34 }, { 120,  60 } };
Vertex clip4[2] = { {  90,  80 }, { 130, 110 } };
Vertex clip5[2] = { { 198, 198 }, { 276, 258 } };
Vertex clip6[2] = { { 221,  80 }, { 251, 101 } };

// our polygons (list of vertices)

Vertex quad1[] = {
    {  20, 120 }, {  20, 140 }, {  40, 140 }, {  40, 120 }
};
int quad1_nv = sizeof(quad1) / sizeof(Vertex);

Vertex quad2[] = {
    {  80, 160 }, {  80, 200 }, {  60, 200 }, {  60, 160 }
};
int quad2_nv = sizeof(quad2) / sizeof(Vertex);

Vertex quad3[] = {
    {  20,  60 }, {  50,  60 }, {  50,  50 }, {  20,  50 }
};
int quad3_nv = sizeof(quad3) / sizeof(Vertex);

Vertex quad4[] = {
    {  44, 122 }, {  60, 122 }, {  60, 146 }, {  44, 146 }
};
int quad4_nv = sizeof(quad3) / sizeof(Vertex);

Vertex quad5[] = {
    {  50,  70 }, {  60,  70 }, {  60, 100 }, {  50, 100 }
};
int quad5_nv = sizeof(quad5) / sizeof(Vertex);

Vertex tri1[] = {
    {  40,  45 }, {  50,  10 }, {  60,  45 }
};
int tri1_nv = sizeof(tri1) / sizeof(Vertex);

Vertex pent1[] = {
    {  80,  20 }, {  90,  10 }, { 110,  20 }, { 100,  50 }, {  80,  40 }
};
int pent1_nv = sizeof(pent1) / sizeof(Vertex);

Vertex hept1[] = {
    { 120,  70 }, { 140,  70 }, { 160,  80 }, { 160, 100 },
    { 140, 110 }, { 120, 100 }, { 110,  90 }
};
int hept1_nv = sizeof(hept1) / sizeof(Vertex);

Vertex nona1[] = {
    { 190,  56 }, { 230,  68 }, { 247,  56 }, { 269,  71 }, { 284, 104 },
    { 251, 122 }, { 233, 110 }, { 212, 119 }, { 203,  95 }
};
int nona1_nv = sizeof(nona1) / sizeof(Vertex);

Vertex deca1[] = {
    { 177, 156 }, { 222, 188 }, { 267, 156 }, { 250, 207 }, { 294, 240 },
    { 240, 240 }, { 222, 294 }, { 204, 240 }, { 150, 240 }, { 194, 207 }
};
int deca1_nv = sizeof(deca1) / sizeof(Vertex);

// count of vertices in each clipped polygon
#define	MAX_POLYS	10
int nv[MAX_POLYS];

///
// Drawing-related variables
///

// dimensions of the drawing window
int w_width  = 300;
int w_height = 300;

// how to calculate an offset into the vertex buffer
#define BUFFER_OFFSET(i)	((char *)NULL + (i))

// our Canvas
Canvas *canvas;

// variables related to drawing the clipping regions
BufferSet clipBuffers;

// variables related to drawing the original polygons
BufferSet polyOutlineBuffers;

// variables related to drawing the resulting polygons
BufferSet clippedPolyBuffers;

// shader program handle
GLuint program;

// "fall back" to GLSL 1.20 shaders?
bool useAltShaders = false;

///
// Support function that draws clip regions as line loops
///
void drawClipRegion( Vertex ll, Vertex ur, Canvas *C ) {
    canvasSetPixel( ll.x, ll.y, C );  // LL
    canvasSetPixel( ur.x, ll.y, C );  // LR
    canvasSetPixel( ur.x, ur.y, C );  // UR
    canvasSetPixel( ll.x, ur.y, C );  // UL
}

///
// Draw all the clipping rectangles
///
void makeClipOutlines( Canvas *C ) {

    // we draw the clipping regions as white rectangles.
    // all vertices are put into one vertex buffer, and
    // we use glDrawArrays() instead of glDrawElements()
    // to draw them as line loops

    drawClipRegion( clip1[0], clip1[1], C );
    drawClipRegion( clip2[0], clip2[1], C );
    drawClipRegion( clip3[0], clip3[1], C );
    drawClipRegion( clip4[0], clip4[1], C );
    drawClipRegion( clip5[0], clip5[1], C );
    drawClipRegion( clip6[0], clip6[1], C );
}

///
// Draw a polygon
///
void drawPolygon( Vertex v[], int nv, Canvas *C ) {

    // just put the vertices into the vertex buffer, in order

    for( int i = 0; i < nv; ++i ) {
        canvasSetPixel( v[i].x, v[i].y, C );
    }
}

///
// Create the polygon outlines
///
void makePolygonOutlines( Canvas *C ) {

    // here, we draw the original polygons; these
    // will be rendered using line loops

    canvasSetColor( 1.0f, 0.0f, 0.0f, C );       // red
    drawPolygon( quad1, quad1_nv, C );
    canvasSetColor( 0.0f, 1.0f, 0.0f, C );       // green
    drawPolygon( quad2, quad2_nv, C );
    canvasSetColor( 0.0f, 0.0f, 1.0f, C );       // blue
    drawPolygon( quad3, quad3_nv, C );
    canvasSetColor( 1.0f, 0.0f, 1.0f, C );       // magenta
    drawPolygon( quad4, quad4_nv, C );
    canvasSetColor( 1.0f, 0.5f, 1.0f, C );       // red-greenish-blue
    drawPolygon( pent1, pent1_nv, C );
    canvasSetColor( 0.7f, 0.7f, 0.7f, C );       // gray
    drawPolygon( hept1, hept1_nv, C );
    canvasSetColor( 0.871f, 0.722f, 0.529f, C ); // burlywood
    drawPolygon( nona1, nona1_nv, C );
    canvasSetColor( 1.0f, 0.64705f, 0.0f, C );	 // orange
    drawPolygon( deca1, deca1_nv, C );
    canvasSetColor( 1.0f, 1.0f, 0.0f, C );       // yellow
    drawPolygon( quad5, quad5_nv, C );
    canvasSetColor( 0.6f, 0.196f, 0.8f, C );     // dark orchid
    drawPolygon( tri1, tri1_nv, C );
}

///
// Create the filled polygons
///
void makePolygons( Canvas *C ) {
    // temporary vertex array (big enough for anything?)
    Vertex tmp[100];

    ///
    // first polygon:  entirely within region
    ///

    canvasSetColor( 1.0f, 0.0f, 0.0f, C );	// red
    nv[0] = clipPolygon( quad1_nv, quad1, tmp, clip1[0], clip1[1] );
    if( nv[0] > 0 ) {
        drawPolygon( tmp, nv[0], C );
    }

    ///
    // second polygon:  entirely outside region
    ///

    canvasSetColor( 0.0f, 1.0f, 0.0f, C ); // green
    nv[1] = clipPolygon( quad2_nv, quad2, tmp, clip1[0], clip1[1] );
    // shouldn't draw anything!
    if( nv[1] > 0 ) {
        drawPolygon( tmp, nv[1], C );
    }

    ///
    // third polygon:  halfway outside on left
    ///

    canvasSetColor( 0.0f, 0.0f, 1.0f, C );	// blue
    nv[2] = clipPolygon( quad3_nv, quad3, tmp, clip2[0], clip2[1] );
    if( nv[2] > 0 ) {
        drawPolygon( tmp, nv[2], C );
    }

    ///
    // fourth polygon:  part outside on right
    ///

    canvasSetColor( 1.0f, 0.0f, 1.0f, C );	// magenta
    nv[3] = clipPolygon( quad4_nv, quad4, tmp, clip1[0], clip1[1] );
    if( nv[3] > 0 ) {
        drawPolygon( tmp, nv[3], C );
    }

    ///
    // fifth polygon:  outside on left and bottom
    ///

    canvasSetColor( 1.0f, 0.5f, 1.0f, C ); // red-greenish-blue
    nv[4] = clipPolygon( pent1_nv, pent1, tmp, clip3[0], clip3[1] );
    if( nv[4] > 0 ) {
        drawPolygon( tmp, nv[4], C );
    }

    ///
    // sixth polygon:  outside on top, right, and bottom
    ///

    canvasSetColor( 0.7f, 0.7f, 0.7f, C );	// gray
    nv[5] = clipPolygon( hept1_nv, hept1, tmp, clip4[0], clip4[1] );
    if( nv[5] > 0 ) {
        drawPolygon( tmp, nv[5], C );
    }

    ///
    // seventh polygon:  surrounds the clip region
    ///

    canvasSetColor( 0.871f, 0.722f, 0.529f, C ); // burlywood
    nv[6] = clipPolygon( nona1_nv, nona1, tmp, clip6[0], clip6[1] );
    if( nv[6] > 0 ) {
        drawPolygon( tmp, nv[6], C );
    }

    ///
    // eighth polygon:  outside on all four edges
    ///

    canvasSetColor( 1.0f, 0.64705f, 0.0f, C );	// orange
    nv[7] = clipPolygon( deca1_nv, deca1, tmp, clip5[0], clip5[1] );
    if( nv[7] > 0 ) {
        drawPolygon( tmp, nv[7], C );
    }

    ///
    // ninth polygon:  outside on top
    ///

    canvasSetColor( 1.0f, 1.0f, 0.0f, C );	// yellow
    nv[8] = clipPolygon( quad5_nv, quad5, tmp, clip2[0], clip2[1] );
    if( nv[8] > 0 ) {
        drawPolygon( tmp, nv[8], C );
    }

    ///
    // tenth polygon:  outside on bottom
    ///

    canvasSetColor( 0.6f, 0.196f, 0.8f, C );	// dark orchid
    nv[9] = clipPolygon( tri1_nv, tri1, tmp, clip2[0], clip2[1] );
    if( nv[9] > 0 ) {
        drawPolygon( tmp, nv[9], C );
    }

}

///
// Create all our objects
///
void createImage( Canvas *C )
{
    // start with a clean canvas
    canvasClear( C );

    // first, create the clipping region buffers
    //
    // start by putting all the vertices for all
    // the regions into a single set of buffers

    // draw all of them in white
    canvasSetColor( 1.0f, 1.0f, 1.0f, C );
    makeClipOutlines( C );

    // collect the vertex, element, and color data for these
    createBuffers( &clipBuffers, C );

    // next, do the polygon outlines

    canvasClear( C );
    makePolygonOutlines( C );
    createBuffers( &polyOutlineBuffers, C );

    // finally, do the polygons

    canvasClear( C );
    makePolygons( C );
    createBuffers( &clippedPolyBuffers, C );

}

///
// OpenGL initialization
///
void init( void )
{
    // Create our Canvas "object"
    canvas = canvasCreate( w_width, w_height );

    if( canvas == NULL ) {
        fputs( "error - cannot create Canvas\n", stderr );
	glfwTerminate();
        exit( 1 );
    }

    // Load shaders and use the resulting shader program
    ShaderError error;
    if( useAltShaders ) {
        program = shaderSetup( "alt.vert", "alt.frag", &error );
    } else {
        program = shaderSetup( "shader.vert", "shader.frag", &error );
    }
    if( !program ) {
        fprintf( stderr, "Error setting up shaders - %s\n",
	    errorString(error) );
	glfwTerminate();
	exit( 1 );
    }

    // OpenGL state initialization
    glEnable( GL_DEPTH_TEST );
    // glEnable( GL_CULL_FACE );
    // glCullFace( GL_BACK );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );

    // create the geometry for our shapes.
    createImage( canvas );
}

///
// selectBuffers() - bind the correct vertex and element buffers
//
// @param program - GLSL program object
// @param B       - the BufferSet to update
///
void selectBuffers( GLuint program, BufferSet *B ) {

    // bind the buffers
    glBindBuffer( GL_ARRAY_BUFFER, B->vbuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, B->ebuffer );

    // set up the vertex attribute variables
    GLuint vPosition = glGetAttribLocation( program , "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(B->vSize) );
}

///
// Display the current image
///
void display( void )
{
    // clear the frame buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // ensure we have selected the correct shader program
    glUseProgram( program );

    // set up our scale factors for normalization
    GLuint sf = glGetUniformLocation( program, "sf" );
    glUniform2f( sf, 2.0f / (w_width - 1.0f), 2.0f / (w_height - 1.0f) );

    ///
    // first, draw the polygon outlines
    ///

    // bind our buffers
    selectBuffers( program, &polyOutlineBuffers );

    // draw our shapes
    int skip = 0;
    glDrawArrays( GL_LINE_LOOP, skip, quad1_nv ); skip += quad1_nv;
    glDrawArrays( GL_LINE_LOOP, skip, quad2_nv ); skip += quad2_nv;
    glDrawArrays( GL_LINE_LOOP, skip, quad3_nv ); skip += quad3_nv;
    glDrawArrays( GL_LINE_LOOP, skip, quad4_nv ); skip += quad4_nv;
    glDrawArrays( GL_LINE_LOOP, skip, pent1_nv ); skip += pent1_nv;
    glDrawArrays( GL_LINE_LOOP, skip, hept1_nv ); skip += hept1_nv;
    glDrawArrays( GL_LINE_LOOP, skip, nona1_nv ); skip += nona1_nv;
    glDrawArrays( GL_LINE_LOOP, skip, deca1_nv ); skip += deca1_nv;
    glDrawArrays( GL_LINE_LOOP, skip, quad5_nv ); skip += quad5_nv;
    glDrawArrays( GL_LINE_LOOP, skip, tri1_nv  ); skip += tri1_nv;

    ///
    // next, draw the clipped polygons
    ///

    // bind our buffers
    selectBuffers( program, &clippedPolyBuffers );

    // draw our shapes
    //
    // be sure to only draw what's there
    skip = 0;
    for( int i = 0; i < MAX_POLYS; ++i ) {
        if( nv[i] ) {
            glDrawArrays( GL_TRIANGLE_FAN, skip, nv[i] ); skip += nv[i];
        }
    }

    ///
    // finally, draw the clip region outlines (we do these
    // last so that they show up on top of everything else)
    ///

    // bind our buffers
    selectBuffers( program, &clipBuffers );

    // draw our shapes
    glDrawArrays( GL_LINE_LOOP,  0, 4 );
    glDrawArrays( GL_LINE_LOOP,  4, 4 );
    glDrawArrays( GL_LINE_LOOP,  8, 4 );
    glDrawArrays( GL_LINE_LOOP, 12, 4 );
    glDrawArrays( GL_LINE_LOOP, 16, 4 );
    glDrawArrays( GL_LINE_LOOP, 20, 4 );
}

///
// Event callback routines
///

///
// Handle keyboard input
///
void keyboard( GLFWwindow *window, int key, int scan, int action, int mods )
{
    switch( key ) {
        case GLFW_KEY_ESCAPE:
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose( window, 1 );
            break;
    }
}

///
// Error callback for GLFW
///
void glfwError( int code, const char *desc )
{
    fprintf( stderr, "GLFW error %d: %s\n", code, desc );
    exit( 2 );
}

///
// Main program for polygon clip assignment
///
int main( int argc, char *argv[] )
{
    glfwSetErrorCallback( glfwError );

    if( !glfwInit() ) {
        fputs( "Can't initialize GLFW!\n", stderr );
        exit( 1 );
    }

    GLFWwindow *window = glfwCreateWindow( w_width, w_height,
        "Lab 3 - Polygon Clipping", NULL, NULL );

    if( !window ) {
        fputs( "GLFW window create failed!\n", stderr );
	glfwTerminate();
	exit( 1 );
    }

    glfwMakeContextCurrent( window );

#ifndef __APPLE__
    GLenum err = glewInit();
    if( err != GLEW_OK ) {
        fprintf( stderr, "GLEW error: %s\n", glewGetErrorString(err) );
        glfwTerminate();
        exit( 1 );
    }

    if( !GLEW_VERSION_3_2 ) {
        fputs( "GLEW: OpenGL 3.2 not available\n", stderr );
	if( !GLEW_VERSION_2_1 ) {
	    fputs( "GLEW: OpenGL 2.1 not available, either!\n", stderr );
            glfwTerminate();
            exit( 1 );
	}
    }
#endif

    // determine whether or not we can use GLSL 1.50
    int maj = glfwGetWindowAttrib( window, GLFW_CONTEXT_VERSION_MAJOR );
    int min = glfwGetWindowAttrib( window, GLFW_CONTEXT_VERSION_MINOR );

    fprintf( stderr, "GLFW: using %d.%d context\n", maj, min );
    if( maj < 3 || (maj == 3 && min < 2) ) {
        // nope!
	useAltShaders = true;
    }

    init();

    glfwSetKeyCallback( window, keyboard );

    while( !glfwWindowShouldClose(window) ) {
        display();
	glfwSwapBuffers( window );
	glfwWaitEvents();
    }

    glfwDestroyWindow( window );
    glfwTerminate();

    return 0;
}

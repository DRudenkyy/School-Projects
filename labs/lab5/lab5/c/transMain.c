///
//  transMain.c
//
//  Created by Warren R. Carithers 2016/11/10.
//  Based on code created by Joe Geigel.
//  Updated 2018/11/12 by wrc.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Main program for transformation assignment
//
//  This file should not be modified by students.
///

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

#include "Buffers.h"
#include "ShaderSetup.h"
#include "Canvas.h"
#include "Shapes.h"
#include "Viewing.h"

// do we need to do a display() call?
bool updateDisplay = true;

// how to calculate an offset into the vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// our drawing canvas
Canvas *canvas;

// dimensions of the drawing window
int w_width  = 512;
int w_height = 512;

// buffer information
BufferSet shapeBuffers;

// mouse click tracker
int counter = 0;

// flags controlling drawing options
bool cameraAdjust = false;
bool transformsOn = false;
int viewMode = 1;

// GLSL shader program handle
GLuint program;

// shader attribute variable location
GLint vPosition;

// alternate camera parameters
Tuple alt_eye =    { 0.0f, 1.3f, -0.5f };
Tuple alt_lookat = { 0.0f, -0.4f, -1.0f };
Tuple alt_up =     { 0.0f, 1.0f, 0.0f };

// transformation values
Tuple alt_scale =  { 1.0f, 2.0f, 1.0f };
Tuple alt_rotate = { 0.0f, 10.0f, 335.0f };
Tuple alt_xlate =  { -0.2f, 0.2f, 0.0f };

///
// Create the current teapot image
///
void createShape()
{
    // reset the canvas
    canvasClear( canvas );

    // make the teapot
    makeTeapot( canvas );

    // create the necessary buffers
    createBuffers( &shapeBuffers, canvas );
}

///
// OpenGL initialization
///
void init()
{
    // Create our Canvas
    canvas = canvasCreate( w_width, w_height );

    if( canvas == NULL ) {
        fputs( "error - cannot create Canvas\n", stderr );
	glfwTerminate();
        exit( 1 );
    }

    // Load shaders and use the resulting shader program
    ShaderError error;
    program = shaderSetup( "shader.vert", "shader.frag", &error );
    if( !program ) {
        fprintf( stderr, "Error setting up shaders - %s\n",
	    errorString(error) );
	glfwTerminate();
	exit( 1 );
    }

    // select this shader program
    glUseProgram( program );

    // get location of shader global variables
    vPosition = glGetAttribLocation( program, "vPosition" );

    // Other OpenGL initialization
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glClearDepth( 1.0f );

    // create the geometry for your shapes.
    createShape();
}

///
// selectBuffers() - bind the correct vertex and element buffers
//
// @param program - GLSL program object
// @param B       - the BufferSet to use
///
void selectBuffers( GLuint program, BufferSet *B ) {

    // bind the buffers
    glBindBuffer( GL_ARRAY_BUFFER, B->vbuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, B->ebuffer );

    // set up the vertex attribute variable
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    // all other globals must be set by
    // the functions in the Viewing module
}

///
// Display the current image
///
void display( void )
{
    // clear the frame buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // bind our buffers
    selectBuffers( program, &shapeBuffers );

    // set up viewing and projection parameters

    if( viewMode == 1 ) {
        setUpProjection( program, Frustum );
    } else if( viewMode == 2 ) {
        setUpProjection( program, Ortho );
    } else {
        fprintf( stderr, "unknown viewing mode %d - resetting\n", viewMode );
	viewMode = 1;
	setUpProjection( program, Frustum );
    }

    // set up the camera
    //
    // changing the camera sets eyepoint to (0,1.3,-0.5), lookat
    // to (0,-0.4,-1.0), and up to (0,1,0)

    if( cameraAdjust ) {
        setUpCamera( program, alt_eye, alt_lookat, alt_up );
    } else {
        clearCamera( program );
    }

    // set up transformations
    //
    // transformations are applied in this order (if you are having
    // trouble recreating the solution image, check your order of
    // matrix multiplication):
    //
    //    scale Y by 2
    //    rotate around Z by 335 degrees
    //    rotate around Y by 10 degrees
    //    translate in X by -0.2
    //    translate in Y by 0.2

    if( transformsOn ) {
        setUpTransforms( program, alt_scale, alt_rotate, alt_xlate );
    } else {
        clearTransforms( program );
    }

    // draw your shape
    glDrawElements( GL_TRIANGLES, shapeBuffers.numElements,
                    GL_UNSIGNED_INT, (void *)0 );
}

///
// Keyboard callback
//
// We don't need to distinguish between (e.g.) 'a' and 'A', so
// we can use the simpler form of callback
///
void keyboard( GLFWwindow *window, int key, int scan, int action, int mods )
{
    if( action != GLFW_PRESS )  // only activate on press, not rpt/release
        return;

    switch( key ) {
	case GLFW_KEY_F:
            viewMode = 1;
	    updateDisplay = true;
            break;

	case GLFW_KEY_O:
            viewMode = 2;
	    updateDisplay = true;
            break;

        case GLFW_KEY_ESCAPE:
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose( window, 1 );
            break;
    }
}

///
// Callback for mouse button clicks
///
void mouse( GLFWwindow *window, int button, int action, int mods )
{
    if( action != GLFW_PRESS )	// only activate on press, not release
        return;

    if( button == GLFW_MOUSE_BUTTON_LEFT )
        counter++;

    switch( counter % 4 ) {
        case 0:
            // default camera, no transforms
            cameraAdjust = false;
            transformsOn = false;
            break;
        case 1:
            // default camera, turn on transformations
            cameraAdjust = false;
            transformsOn = true;
            break;
        case 2:
            // modified camera, no transforms
            cameraAdjust = true;
            transformsOn = false;
            break;
        case 3:
            // modified camera, transformations
            cameraAdjust = true;
            transformsOn = true;
            break;
    }
    updateDisplay = true;
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
// Main program for transformation assignment
///
int main( int argc, char **argv ) {

    glfwSetErrorCallback( glfwError );

    if( !glfwInit() ) {
        fputs( "Can't initialize GLFW!\n", stderr );
        exit( 1 );
    }

    GLFWwindow *window = glfwCreateWindow( w_width, w_height,
        "Lab 5 - Transformations", NULL, NULL );

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
        fputs( "*** GLSL 1.50 shaders may not compile\n", stderr );
    }

    init();

    glfwSetKeyCallback( window, keyboard );
    glfwSetMouseButtonCallback( window, mouse );

    while( !glfwWindowShouldClose(window) ) {
	if( updateDisplay ) {
	    updateDisplay = false;
            display();
            glfwSwapBuffers( window );
	}
        glfwWaitEvents();
    }

    glfwDestroyWindow( window );
    glfwTerminate();

    return 0;
}

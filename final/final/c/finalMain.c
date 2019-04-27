///
//  finalMain.cpp
//
//  Created by Jake Brandt 2019/04/26.
//  Based on code created by Joe Geigel and Warren Carithers for lab 6.
//  Copyright 2019 Rochester Institute of Technology.  All rights reserved.
//
//  Main program for final project.
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

// do we need to do a display() call?
bool updateDisplay = true;

// How to calculate an offset into the vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// our drawing canvas
Canvas *canvas;

// dimensions of the drawing window
int w_width  = 600;
int w_height = 600;

// Animation flag
bool animating = false;

///
// OpenGL initialization
///
void init( void )
{
    // Create our Canvas
    canvas = canvasCreate( w_width, w_height );

    if( canvas == NULL ) {
        fputs( "error - cannot create Canvas\n", stderr );
        glfwTerminate();
        exit( 1 );
    }

    // Other OpenGL initialization
    glEnable( GL_DEPTH_TEST );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );
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

    // set up the vertex attribute variables
    GLint vPosition = glGetAttribLocation( program , "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );
    int offset = B->vSize;

    if( B->cSize ) {  // color data
        GLint vColor = glGetAttribLocation( program, "vColor" );
        glEnableVertexAttribArray( vColor );
        glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(offset) );
        offset += B->cSize;
    }

    if( B->nSize ) {  // normal data
        GLint vNormal = glGetAttribLocation( program, "vNormal" );
        glEnableVertexAttribArray( vNormal );
        glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(offset) );
        offset += B->nSize;
    }

    if( B->tSize ) {  // texture coordinate data
        GLint vTexCoord = glGetAttribLocation( program, "vTexCoord" );
        glEnableVertexAttribArray( vTexCoord );
        glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(offset) );
        offset += B->tSize;
    }
}

///
// Display the current image
///
void display( void )
{
    // clear and draw params..
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

///
// Event callback routines
///

///
// Handle keyboard input
//
// We don't need to distinguish between (e.g.) 'a' and 'A', so
// we can use the simpler form of callback
///
void keyboard( GLFWwindow *window, int key, int scan, int action, int mods )
{
    if( action != GLFW_PRESS )   // only activate on press, not rpt/release
        return;

    switch( key ) {

        case GLFW_KEY_A:    // animate
            animating = true;
            break;

        case GLFW_KEY_S:    // stop animating
            animating = false;
            break;

        case GLFW_KEY_ESCAPE:   // terminate the program
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose( window, 1 );
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
// Main program for texting assignment
///
int main( int argc, char **argv ) {

    glfwSetErrorCallback( glfwError );

    if( !glfwInit() ) {
        fputs( "Can't initialize GLFW!\n", stderr );
        exit( 1 );
    }

    GLFWwindow *window = glfwCreateWindow( w_width, w_height,
        "Final Project - CSCI 510", NULL, NULL );

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
        fputs( "*** GLSL 1.50 shaders may not compile\n", stderr );
    }

    init();

    glfwSetKeyCallback( window, keyboard );

    //
    // Main display/render loop

    while( !glfwWindowShouldClose(window) ) {
        // Render loops are where you'd implement any physics or
        // animation stepping in addition to the rendering.

        if( updateDisplay ) {
            updateDisplay = false;
            display();
            glfwSwapBuffers( window );
        }
        glfwPollEvents();
    }

    glfwDestroyWindow( window );
    glfwTerminate();

    return 0;
}

///
//  textingMain.cpp
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on code created by Joe Geigel.
//  Updated 2018/11/28 by wrc.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Main program for lighting/shading/texturing assignment
//
//  This file should not be modified by students.
///

#include <cstdlib>
#include <iostream>

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
#include "Sphere.h"
#include "Viewing.h"
#include "Lighting.h"
#include "Textures.h"

using namespace std;

// do we need to do a display() call?
bool updateDisplay = true;

// How to calculate an offset into the vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// our drawing canvas
Canvas *canvas;

// dimensions of the drawing window
int w_width  = 600;
int w_height = 600;

//
// We need vertex buffers and element buffers for the
// quad (texture mapped), the teapot (Phong shaded),
// and the sphere (Phong shaded, by default).
//
BufferSet quadBuffers;
BufferSet teapotBuffers;
BufferSet sphereBuffers;
BufferSet coneBuffers;

// Animation flag
bool animating = false;

// Initial animation rotation angles for the objects
GLfloat angles = 0.0f;
GLfloat xzAngles = 0.0f;
bool flip = false;

// Initial translation factors for the sphere
#define XLATE_X    1.3f
#define XLATE_Y    2.2f
#define XLATE_Z    -1.5f
GLfloat xlate[3] = { XLATE_X, XLATE_Y, XLATE_Z };

// Current state of animation for the sphere
int sphereState = 0;

// program IDs...for shader programs
GLuint pshader, tshader;

///
// createShape() - create vertex and element buffers for a shape
//
// @param obj - which shape to create
// @param C   - the Canvas to use
///
void createShape( int obj, Canvas &C )
{
    // clear any previous shape
    C.clear();

    // make the shape
    switch( obj ) {
        case OBJ_QUAD:    makeQuad( C ); break;
	case OBJ_SPHERE:  makeSphere( C ); break;
	case OBJ_TEAPOT:  makeTeapot( C ); break;
	case OBJ_CONE: makeCone(C, 2, 20, 20); break;
	default:          makeTeapot( C ); break;
    }

    // create the necessary buffers
    if( obj == OBJ_QUAD ) {
        quadBuffers.createBuffers( C );
    } else if( obj == OBJ_SPHERE ) {
        sphereBuffers.createBuffers( C );
    } else if(obj == OBJ_TEAPOT) {
        teapotBuffers.createBuffers( C );
    } else {
		coneBuffers.createBuffers( C );
	}
}

///
// OpenGL initialization
///
void init( void )
{
    // Create our Canvas
    canvas = new Canvas( w_width, w_height );

    if( canvas == NULL ) {
        cerr << "error - cannot create Canvas" << endl;
        glfwTerminate();
        exit( 1 );
    }

    // Load texture image(s)
    loadTextures();

    // Load shaders, verifying each
    ShaderError error;
    tshader = shaderSetup( "texture.vert", "texture.frag", &error );
    if( !tshader ) {
        cerr << "Error setting up texture shader - " <<
            errorString(error) << endl;
        glfwTerminate();
        exit( 1 );
    }

    pshader = shaderSetup( "phong.vert", "phong.frag", &error );
    if( !pshader ) {
        cerr << "Error setting up Phong shader - " <<
            errorString(error) << endl;
        glfwTerminate();
        exit( 1 );
    }

    // Other OpenGL initialization
    glEnable( GL_DEPTH_TEST );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );

    // Create all our objects
    createShape( OBJ_QUAD, *canvas );
    createShape( OBJ_TEAPOT, *canvas );
    createShape( OBJ_SPHERE, *canvas );
    createShape( OBJ_CONE, *canvas );
}

///
// selectBuffers() - bind the correct vertex and element buffers
//
// @param program - GLSL program object
// @param B       - the BufferSet to use
///
void selectBuffers( GLuint program, BufferSet &B ) {

    // bind the buffers
    glBindBuffer( GL_ARRAY_BUFFER, B.vbuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, B.ebuffer );

    // set up the vertex attribute variables
    GLint vPosition = glGetAttribLocation( program , "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );
    int offset = B.vSize;

    if( B.cSize ) {  // color data
        GLint vColor = glGetAttribLocation( program, "vColor" );
        glEnableVertexAttribArray( vColor );
        glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(offset) );
        offset += B.cSize;
    }

    if( B.nSize ) {  // normal data
        GLint vNormal = glGetAttribLocation( program, "vNormal" );
        glEnableVertexAttribArray( vNormal );
        glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(offset) );
        offset += B.nSize;
    }

    if( B.tSize ) {  // texture coordinate data
        GLint vTexCoord = glGetAttribLocation( program, "vTexCoord" );
        glEnableVertexAttribArray( vTexCoord );
        glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(offset) );
        offset += B.tSize;
    }
}

///
// Display the current image
///
void display( void )
{
    // clear and draw params..
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // first, the quad
    drawShape( tshader, OBJ_QUAD, quadBuffers );

    // next, the sphere
    drawSphere( pshader, tshader, sphereBuffers );

    // now, draw the teapot
    drawShape( pshader, OBJ_TEAPOT, teapotBuffers );
    
    drawShape( pshader, OBJ_CONE, coneBuffers );

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
    if( action != GLFW_PRESS )    // only activate on press, not rpt/release
        return;

    switch( key ) {

        case GLFW_KEY_A:    // animate
            animating = true;
            break;

        case GLFW_KEY_S:    // stop animating
            animating = false;
            break;

        case GLFW_KEY_R:    // reset transformations
            angles = 0.0f;
            xzAngles = 0.0f;
            xlate[0] = XLATE_X;
            flip = false;
	    xlate[1] = XLATE_Y;
	    xlate[2] = XLATE_Z;
	    sphereState = 0;
            break;

        case GLFW_KEY_ESCAPE:   // terminate the program
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose( window, 1 );
            break;
    }

    updateDisplay = true;
}

///
// Animation routine
///
void animate( void ) {

    if( animating ) {

	// first, rotation for the objects
        angles += 1.0f;
	if( angles >= 360.0f ) {
	    angles = 0.0f;
	}
	
	if(!flip) {
		if(xzAngles <= 15)
			xzAngles += 0.05555f;
		else
			flip = true;
	}
	else {
		if(xzAngles >= -15)
			xzAngles -= 0.05555f;
		else
			flip = false;
	}

	// next, translation for the sphere
	switch( sphereState ) {

	    case 0:  // first side
		// horizontal move until tx reaches 0.5
	        if( xlate[0] > 0.5f ) {
		    xlate[0] -= 0.025f;
		    break;
		} else {
		    // fall through into state 1
		    sphereState = 1;
		}

	    case 1:  // second side
		// move downward to the right until ty reaches 1.4
	        if( xlate[1] > 1.4f ) {
		    xlate[0] += 0.0125;
		    xlate[1] -= 0.025f;
		    break;
		} else {
		    // fall through into state 2
		    sphereState = 2;
		}

	    case 2:  // third side
		// move upward to the right until tx reaches 1.3
	        if( xlate[0] < 1.3f ) {
		    xlate[0] += 0.0125;
		    xlate[1] += 0.025f;
		    break;
		}
		// at that point, we fall back to state 0

	    default:  // unknown state - fall back!
		// move back to state 0
		sphereState = 0;
		xlate[0] = XLATE_X;
		xlate[1] = XLATE_Y;

	}

        updateDisplay = true;
    }
}

///
// Error callback for GLFW
///
void glfwError( int code, const char *desc )
{
    cerr << "GLFW error " << code << ": " << desc << endl;
    exit( 2 );
}

///
// Main program for texting assignment
///
int main( int argc, char **argv ) {

    glfwSetErrorCallback( glfwError );

    if( !glfwInit() ) {
        cerr << "Can't initialize GLFW!" << endl;
        exit( 1 );
    }

    GLFWwindow *window = glfwCreateWindow( w_width, w_height,
        "Lab 6 - Shading and Texturing", NULL, NULL );

    if( !window ) {
        cerr << "GLFW window create failed!" << endl;
        glfwTerminate();
        exit( 1 );
    }

    glfwMakeContextCurrent( window );

#ifndef __APPLE__
    GLenum err = glewInit();
    if( err != GLEW_OK ) {
        cerr << "GLEW error: " << glewGetErrorString(err) << endl;
        glfwTerminate();
        exit( 1 );
    }
    if( !GLEW_VERSION_3_2 ) {
        cerr << "GLEW: OpenGL 3.2 not available" << endl;
        if( !GLEW_VERSION_2_1 ) {
            cerr << "GLEW: OpenGL 2.1 not available, either!" << endl;
            glfwTerminate();
            exit( 1 );
        }
    }
#endif

    // determine whether or not we can use GLSL 1.50
    int maj = glfwGetWindowAttrib( window, GLFW_CONTEXT_VERSION_MAJOR );
    int min = glfwGetWindowAttrib( window, GLFW_CONTEXT_VERSION_MINOR );

    cerr << "GLFW: using " << maj << "." << min << " context" << endl;
    if( maj < 3 || (maj == 3 && min < 2) ) {
        cerr << "*** GLSL 1.50 shaders may not compile" << endl;
    }

    init();

    glfwSetKeyCallback( window, keyboard );

    int animDelay = 0;

    while( !glfwWindowShouldClose(window) ) {
        if (animDelay > 3000) {
            animDelay = 0;
            animate();
        } else {
            animDelay++;
        }
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

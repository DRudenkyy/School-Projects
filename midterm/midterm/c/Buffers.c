//
//  Buffers.c
//
//  Vertex and element buffer implementations.
//
//  Students should not be modifying this file.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

//
// GLEW and GLFW header files also pull in the OpenGL definitions
//

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "Buffers.h"
#include "Canvas.h"

///
// initBuffer(buf) - reset the supplied BufferSet to its "empty" state
//
// @param buf - which buffer to reset
///
void initBuffer( BufferSet *buf ) {
    buf->vbuffer = buf->ebuffer = 0;
    buf->numElements = 0;
    buf->vSize = buf->eSize = buf->tSize = buf->cSize = 0;
    buf->bufferInit = false;
}

///
// makeBuffer() - make a vertex or element array buffer
//
// @param target - which type of buffer to create
// @param data   - source of data for buffer (or NULL)
// @param size   - desired length of buffer
//
// @return the ID of the new buffer
//
GLuint makeBuffer( GLenum target, const void *data, GLsizei size ) {
    GLuint buffer;

    glGenBuffers( 1, &buffer );
    glBindBuffer( target, buffer );
    glBufferData( target, size, data, GL_STATIC_DRAW );

    return( buffer );
}

///
// createBuffers(buf,pipeline) create a set of buffers for the object
//     currently held in 'pipeline's Canvas
//
// @param B   - the BufferSet to be modified
// @param P   - the Pipeline we'll use for drawing
///
void createBuffers( BufferSet *B, Pipeline *P ) {

    // reset this BufferSet if it has already been used
    if( B->bufferInit ) {
        // must delete the existing buffer IDs first
        glDeleteBuffers( 1, &(B->vbuffer) );
        glDeleteBuffers( 1, &(B->ebuffer) );
	// clear everything out
        initBuffer( B );
    }

    // get the vertices
    B->numElements = pipelineNumVertices( P );
    float *points = pipelineGetVertices( P );
    // #bytes = number of elements * 4 floats/element * bytes/float
    B->vSize = B->numElements * 4 * sizeof(float);
    int vbufSize = B->vSize;

    // get the color data
    float *colors = pipelineGetColors( P );
    B->cSize = B->numElements * 4 * sizeof(float);
    vbufSize += B->cSize;

    // get the element data
    GLuint *elements = pipelineGetElements( P );
    // #bytes = number of elements * bytes/element
    B->eSize = B->numElements * sizeof(GLuint);

    // first, create the connectivity data
    B->ebuffer = makeBuffer( GL_ELEMENT_ARRAY_BUFFER, elements, B->eSize );

    // next, the vertex buffer, containing vertices and "extra" data
    B->vbuffer = makeBuffer( GL_ARRAY_BUFFER, NULL, vbufSize );
    glBufferSubData( GL_ARRAY_BUFFER, 0, B->vSize, points );
    glBufferSubData( GL_ARRAY_BUFFER, B->vSize, B->cSize, colors );

    // NOTE:  'points', 'colors', and 'elements' are dynamically allocated,
    // but we don't free them here because they will be freed at the next
    // call to clear() or the get*() functions

    // finally, mark it as set up
    B->bufferInit = true;
}

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
// dumpBuffer(buf) - dump the contents of a BufferSet
//
// @param which - description of the buffer
// @param buf - the buffer to dump
///
void dumpBuffer( const char *which, BufferSet *buf ) {
    printf( "Dumping buffer %s (%s):\n", which,
        buf->bufferInit ? "initialized" : "not initialized" );
    printf( "  IDs: v %u e %u  #elements: %d\n", buf->vbuffer,
        buf->ebuffer, buf->numElements );
    printf( "  Sizes:  v %ld e %ld t %ld c %ld\n", buf->vSize,
        buf->eSize, buf->tSize, buf->cSize );
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
// createBuffers(buf,canvas) create a set of buffers for the object
//     currently held in 'canvas'.
//
// @param B   - the BufferSet to be modified
// @param C   - the Canvas we'll use for drawing
///
void createBuffers( BufferSet *B, Canvas *C ) {

    // reset this BufferSet if it has already been used
    if( B->bufferInit ) {
        // must delete the existing buffer IDs first
        glDeleteBuffers( 1, &(B->vbuffer) );
        glDeleteBuffers( 1, &(B->ebuffer) );
        initBuffer( B );
    }

    // get the vertices
    B->numElements = canvasNumVertices( C );

    // if there are no vertices, there's nothing for us to do
    if( B->numElements < 1 ) {
        return;
    }

    // OK, we have vertices!
    // #bytes = number of elements * 4 floats/element * bytes/float
    B->vSize = B->numElements * 4 * sizeof(float);
    float *points = canvasGetVertices( C );

    // accumulate the required vertex buffer size
    int vbufSize = B->vSize;

    // get the color data (if there is any)
    float *colors = canvasGetColors( C );
    if( colors != NULL ) {
        B->cSize = B->numElements * 4 * sizeof(float);
        vbufSize += B->cSize;
    }

    // get the element data
    // #bytes = number of elements * bytes/element
    B->eSize = B->numElements * sizeof(GLuint);
    GLuint *elements = canvasGetElements( C );

    // first, create the connectivity data
    B->ebuffer = makeBuffer( GL_ELEMENT_ARRAY_BUFFER, elements, B->eSize );

    // next, the vertex buffer, containing vertices and "extra" data
    B->vbuffer = makeBuffer( GL_ARRAY_BUFFER, NULL, vbufSize );
    glBufferSubData( GL_ARRAY_BUFFER, 0, B->vSize, points );

    // add in the color data (if there is any)
    if( B->cSize > 0 ) {
        glBufferSubData( GL_ARRAY_BUFFER, B->vSize, B->cSize, colors );
    }

    // NOTE:  'points', 'colors', and 'elements' are dynamically allocated,
    // but we don't free them here because they will be freed at the next
    // call to clear() or the get*() functions

    // finally, mark it as set up
    B->bufferInit = true;
}

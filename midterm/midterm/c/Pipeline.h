//
//  Pipeline.h
//
//  Created by Warren R. Carithers on 2016/10/17.
//  Based on a version created by Joe Geigel on 11/30/11.
//  Updated 2018/10/18 by wrc.
//  Copyright 2018 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
//

#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include "Canvas.h"
#include "Vertex.h"

///
// Simple wrapper class for midterm assignment
//
// Only methods in the implementation file whose bodies
// contain the comment
//
//    // YOUR IMPLEMENTATION HERE
//
// are to be modified by students; all of these are
// found at the end of the file.
///

typedef struct st_Pipeline {

    ///
    // our Canvas "object"
    ///
    Canvas *sc;

} Pipeline;

///
// "Inherited" methods from Canvas
//
// These are all implemented as simple wrapper functions that invoke
// the corresponding Canvas function using the Canvas that
// is contained in the supplied Pipeline "object".
///

///
// Clears the canvas using the current color.
//
// @param P - the Pipeline to be used
///
void pipelineClear( Pipeline *P );

///
// Set the pixel Z coordinate
//
// @param d The depth to use
// @param P The Pipeline to use
//
// @return  The old depth value
///
float pipelineSetDepth( float d, Pipeline *P );

///
// Sets the current color.
//
// @param r - The red component of the new color(between 0-1)
// @param g - The green component of the new color (between 0-1)
// @param b - The blue component of the new color (between 0-1);
// @param P - the Pipeline to be used
///
void pipelineSetColor( float r, float g, float b, Pipeline *P );

///
// Writes a pixel using the current color.
//
// @param x - The x coord of the pixel to be set
// @param y - The y coord of the pixel to be set
// @param P - the Pipeline to be used
///
void pipelineSetPixel( int x, int y, Pipeline *P );

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
// @param P The Pipeline to use
///
void pipelineDrawOutline( int n, const int x[], const int y[], Pipeline *P );

///
// retrieve the array of element data from this Pipeline
//
// @param P The Pipeline to be used
///
GLuint *pipelineGetElements( Pipeline *P );

///
// retrieve the array of vertex data from this Pipeline
//
// @param P The Pipeline to be used
///
float *pipelineGetVertices( Pipeline *P );

///
// retrieve the array of color data from this Pipeline
//
// @param P The Pipeline to be used
///
float *pipelineGetColors( Pipeline *P );

///
// retrieve the vertex count from this Pipeline
//
// @param P The Pipeline to be used
///
int pipelineNumVertices( Pipeline *P );

///
// returns outline status of this object
//
// @param P The Pipeline to be used
///
bool pipelineIsOutline( Pipeline *P );

///
// Methods which extend Canvas by providing
// additional functionality related to the midterm
// project requirements.
//
// These methods are to be implemented by the student.
///

///
// Constructor
//
// @param w width of canvas
// @param h height of canvas
///
Pipeline *pipelineCreate( int w, int h );

///
// Destructor
//
// @param P Pipeline to destroy
///
void pipelineDestroy( Pipeline *P );

///
// addPoly - Add a polygon to the canvas.  This method does not draw
//           the polygon, but merely stores it for later drawing.
//           Drawing is initiated by the drawPoly() method.
//
//           Returns a unique integer id for the polygon.
//
// @param p - Array of vertex entries defining the polygon to be added
// @param n - Number of vertices in polygon
// @param P - the Pipeline to be used
//
// @return a unique integer identifier for the polygon
///
int pipelineAddPoly( const Vertex v[], int n, Pipeline *P );

///
// drawPoly - Draw the polygon with the given id.  The polygon should
//            be drawn after applying the current transformation to
//            the vertices of the polygon.
//
// @param polyID - the ID of the polygon to be drawn
// @param P - the Pipeline to be used
///
void pipelineDrawPoly( int polyID, Pipeline *P );

///
// clearTransform - Sets the current transformation to the
//                  identity matrix.
//
// @param P - the Pipeline to be used
///
void pipelineClearTransform( Pipeline *P );
    
///
// translate - Add a translation to the current transformation by
//             premultiplying the appropriate translation matrix to
//             the current transformtion matrix.
//
// @param tx - Amount of translation in x
// @param ty - Amount of translation in y
// @param P - the Pipeline to be used
///
void pipelineTranslate( float tx, float ty, Pipeline *P );

///
// rotate - Add a rotation to the current transformation by premultiplying
//          the appropriate rotation matrix to the current transformtion
//          matrix.
//
// @param degrees - Amount of rotation in degrees
// @param P - the Pipeline to be used
///
void pipelineRotate( float degrees, Pipeline *P );
    
///
// scale - Add a scale to the current transformation by premultiplying
//         the appropriate scaling matrix to the current transformtion
//         matrix.
//
// @param sx - Amount of scaling in x
// @param sy - Amount of scaling in y
// @param P - the Pipeline to be used
///
void pipelineScale( float sx, float sy, Pipeline *P );
    
///
// setClipWindow - Define the clip window.
//
// @param bottom - y coord of bottom edge of clip window (in world coords)
// @param top - y coord of top edge of clip window (in world coords)
// @param left - x coord of left edge of clip window (in world coords)
// @param right - x coord of right edge of clip window (in world coords)
// @param P - the Pipeline to be used
///
void pipelineSetClipWindow( float bottom, float top, float left, float right,
    Pipeline *P );
    
///
// setViewport - Define the viewport.
//
// @param xmin - x coord of lower left of view window (in screen coords)
// @param ymin - y coord of lower left of view window (in screen coords)
// @param width - width of view window (in world coords)
// @param height - width of view window (in world coords)
// @param P - the Pipeline to be used
///
void pipelineSetViewport( int x, int y, int width, int height, Pipeline *P );

#endif

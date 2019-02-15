//
//  Clipper.c
//
//  Created by Warren R. Carithers on 01/15/14.
//  Updated 2018/10/01 by wrc.
//  Based on a C++ version created by Joe Geigel on 11/30/11.
//  Copyright 2014 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
//

#include <stdbool.h>

#include "Vertex.h"
#include "Clipper.h"

///
// Simple module that performs clipping
///

///
// clipPolygon
//
// Clip the polygon with vertex count in and vertices inV against the
// rectangular clipping region specified by lower-left corner ll and
// upper-right corner ur. The resulting vertices are placed in outV.
//
// The routine should return the with the vertex count of polygon
// resulting from the clipping.
//
// @param in    the number of vertices in the polygon to be clipped
// @param inV   the incoming vertex list
// @param outV  the outgoing vertex list
// @param ll    the lower-left corner of the clipping rectangle
// @param ur    the upper-right corner of the clipping rectangle
//
// @return number of vertices in the polygon resulting after clipping
//
///
int clipPolygon( int in, const Vertex inV[], Vertex outV[],
		 Vertex ll, Vertex ur )
{
    // YOUR CODE GOES HERE

    return( 0 );  // remember to return the outgoing vertex count!
}

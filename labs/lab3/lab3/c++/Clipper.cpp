//
//  Clipper.cpp
//
//  Created by Warren R. Carithers on 01/15/14.
//  Updated 2018/10/01 by wrc.
//  Based on a version created by Joe Geigel on 11/30/11.
//  Copyright 2014 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Dmytro Rudenkyy
//

#include "Vertex.h"
#include "Clipper.h"
#include <iostream>

using namespace std;

///
// Simple module that performs clipping
///

///
// Constructor
///
Clipper::Clipper() {
}

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

int Clipper::clipPolygon( int in, const Vertex inV[], Vertex outV[],
                          Vertex ll, Vertex ur )
{
	bool clockwise = true;
	
	if(orientation(inV[0], inV[1], inV[2]) == 2)
		clockwise = false;
	else if(orientation(inV[0], inV[1], inV[2]) == 0)
		cerr << "shouldn't happen";
				
	initializeOutV(in, inV, outV, clockwise);
	
	
    return( in );  // remember to return the outgoing vertex count!

}
	//for(int i = 0; i < in; i++)
	//{
		//cerr << i << ": (" << outV[i].x << ", " << outV[i].y << ")" << endl;
	//}
	//cerr << endl;

// To find orientation of ordered triplet (v1, v2, v3). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int Clipper::orientation(Vertex v1, Vertex v2, Vertex v3) 
{ 
    int val = (v2.y - v1.y) * (v3.x - v2.x) - 
              (v2.x - v1.x) * (v3.y - v2.y); 
  
    if (val == 0) return 0;  // colinear 
  
    return (val > 0)? 1: 2; // clock or counterclock wise 
} 

//initializes vertices before clipping by orienting input vertices
//to change to clockwise orientation if counterclockwise originally
void Clipper::initializeOutV(int in, const Vertex inV[], Vertex outV[], bool cc)
{
	//first vertex stays the same regardless of orientation
	outV[0] = inV[0];

	if(cc)
	{
		for(int i = 1; i < in; i++)
		{
			outV[i] = inV[i];
		}
	}
	else
	{
		for(int i = in - 1; i > 0; i--)
		{
			outV[i] = inV[in - i];
		}
	}
}

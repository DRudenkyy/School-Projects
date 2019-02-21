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
	vertexList = {};
	
	bool clockwise = true;
	
	setNewClipParams(ll, ur);

	Vertex orientedVertices[in];
	
	if(orientation(inV[0], inV[1], inV[2]) == 2)
		clockwise = false;
	else if(orientation(inV[0], inV[1], inV[2]) == 0)
		cerr << "shouldn't happen";
				
	orientInitialVertices(in, inV, orientedVertices, clockwise);
	
	
    return( in );  // remember to return the outgoing vertex count!

}

void Clipper::tester(Vertex ll, Vertex ur)
{	
	setNewClipParams(ll, ur);
	Vertex vInside = {150, 150};
	Vertex vAbove = {150, 250};
	Vertex vBelow = {150, 50};
	Vertex vRight = {250, 150};
	Vertex vLeft = {50, 150};
	
	cerr << computeCode(vInside) << endl;
	cerr << computeCode(vAbove) << endl;
	cerr << computeCode(vBelow) << endl;
	cerr << computeCode(vRight) << endl;
	cerr << computeCode(vLeft) << endl;


}

//set the new global clipping parameters
void Clipper::setNewClipParams(Vertex ll, Vertex ur)
{
	clip_xmin = ll.x;
	clip_ymin = ll.y;
	clip_xmax = ur.x;
	clip_ymax = ur.y;
}

// Function to compute region code for a vertex(x, y) 
int Clipper::computeCode(Vertex v) 
{ 
    // initialized as being inside  
    int code = INSIDE;
  
    if (v.x < clip_xmin)       // to the left of rectangle 
        code |= LEFT; 
    else if (v.x > clip_xmax)  // to the right of rectangle 
        code |= RIGHT; 
    if (v.y < clip_ymin)       // below the rectangle 
        code |= BOTTOM; 
    else if (v.y > clip_ymax)  // above the rectangle 
        code |= TOP; 
  
    return code; 
}

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
void Clipper::orientInitialVertices(int in, const Vertex inV[], 
											Vertex oV[], bool cc)
{
	//first vertex stays the same regardless of orientation
	oV[0] = inV[0];

	if(cc)
	{
		for(int i = 1; i < in; i++)
			oV[i] = inV[i];
	}
	else
	{
		for(int i = in - 1; i > 0; i--)
			oV[i] = inV[in - i];
	}
}

void Clipper::printOutV(Vertex arrayV[], int length)
{
	for(int i = 0; i < length; i++)
	{
		cerr << i << ": (" << arrayV[i].x << ", " << arrayV[i].y 
														<< ")" << endl;
	}
	cerr << endl;
}

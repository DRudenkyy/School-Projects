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
	setBoundaries(ll, ur);
	bool clockwise = true;
	//this list will orient the original vertices to get a clockwise orientation
	//it is also updated each time a clipping boundary vertices are calculated
	//that will be used in the next iteration
	Vertex orientedVertices[50];
	
	if(orientation(inV[0], inV[1], inV[2]) == 2)
		clockwise = false;
	else if(orientation(inV[0], inV[1], inV[2]) == 0)
		cerr << "shouldn't happen";
				
	orientInitialVertices(in, inV, orientedVertices, clockwise);

	int outLength = 0;
	int bCase = 0;
	while(bCase < 4)
	{
		outLength = 0;   Vertex p = orientedVertices[in - 1]; 
		for (int j = 0; j < in; j++) {
			Vertex s = orientedVertices[j];

			if( inside( s, boundaries[bCase], bCase ) ) {
				// Cases 1 & 4
				if ( inside( p, boundaries[bCase], bCase ) ) {
					// Case 1
					//add s to outVertices and increment outLength
					outV[outLength] = s;
					outLength ++;
				} else {
					// Case 4
					Vertex i = intersect( p, s, boundaries[bCase], bCase );
					//add i to outVertices and increment outLength
					outV[outLength] = i;
					outLength ++;
					
					//add s to outVertices and increment outLength
					outV[outLength] = s;
					outLength ++;
					}
			} else {
				// Cases 2 & 3
				if( inside ( p, boundaries[bCase], bCase ) ) {
					// Case 2
					Vertex i = intersect( p, s, boundaries[bCase], bCase);
					//add i to outVertices, increment outLength
					outV[outLength] = i;
					outLength ++;
				}
				// Case 3 adds nothing
			}
			p = s;
		}
			//we have more boundaries to clip on so set next input array
			if(bCase < 3)
			{
				//update our in value and set input equal to the vertices 
				//we just generated to be used for the next clipping boundary
				in = outLength;
				for(int i = 0; i < in; i++)
					orientedVertices[i] = outV[i];
			}
			
			bCase ++;
	}
	
    return(outLength);  // remember to return the outgoing vertex count!
    
}

void Clipper::setBoundaries(Vertex ll, Vertex ur)
{
	boundaries[0] = ur.y;	//top
	boundaries[1] = ur.x;	//right
	boundaries[2] = ll.y;	//bottom
	boundaries[3] = ll.x;	//left
}

// compute intersection along PS, return Vertex i
Vertex Clipper::intersect(Vertex p, Vertex s, float boundary, int bCase)
{
	Vertex i = {0, 0};
	switch (bCase) {
		//top or bottom
		case 0:
		case 2:
			//vertical line, only need to set y to boundary
			if(p.x == s.x)
				i = {p.x, boundary};
			else {
				//calculating y is as easy as setting it to boundary
				i.y = boundary;
				//calc x
				i.x = p.x + (boundary - p.y) *
                    (s.x - p.x) / (s.y - p.y);
			}
			break;
		//right or left
		case 1:
		case 3:
			//horizontal line, only need to set x to boundary
			if(p.y == s.y)
				i = {boundary, p.y};
			else {
				//calculating x is as easy as setting it to boundary
				i.x = boundary;
				//calc y
				i.y = p.y + (boundary - p.x) *
                    (s.y - p.y) / (s.x - p.x);
			}
			break;
		default:
			cerr << " Unknown Boundary: " << bCase;
	}
	
	return i;
}
//returns if the point is inside the boundary
//v- vertex we're looking at
//boundary- value of the boundary (x or y)
//bCase- which boundary are we looking at (top right left or bot)
bool Clipper::inside(Vertex v, float boundary, int bCase)
{
	switch (bCase) {
		//top
		case 0:
			return v.y <= boundary;
			break;
		//right
		case 1:
			return v.x <= boundary;
			break;
		//bottom
		case 2:
			return v.y >= boundary;
			break;
		//left
		case 3:
			return v.x >= boundary;
			break;
		default:
			cerr << " Unknown Boundary: " << bCase;
	}
	return false;
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

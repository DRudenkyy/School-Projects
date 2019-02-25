//
//  Clipper.h
//
//  Created by Warren R. Carithers on 02/27/14.
//  Updated 2018/10/01 by wrc.
//  Based on a version created by Joe Geigel on 11/30/11.
//  Copyright 2011 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Dmytro Rudenkyy
//

#ifndef _CLIPPER_H_
#define _CLIPPER_H_

#include "Vertex.h"
#include <list>

///
// Simple module that performs clipping
///

class Clipper {

public:
	
	//main list that will be built up as initial vertices are processed 
	//against the clip area and 
	std::list<Vertex> vertexList = {};
	
	//at first I created a Boundaries struct that kept track of lower and
	//upper vertices that represented it's edge and overloaded == to be 
	//able to compare the boundary being passed in to the one I needed
	//to do arithemtic on. Then I realized that all I needed to keep track
	//of was its x value (if right or left boundary) or y value (otherwise)
	//and when to check vertex < boundary (inside for right and top)
	//and vertex > boundary (inside for left and bottom).
	float boundaries[4];	//top right bottom left

    ///
    // Constructor
    ///
    Clipper();

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
		     Vertex ll, Vertex ur );
		     
	// To find orientation of ordered triplet (v1, v2, v3). 
	// The function returns following values 
	// 0 --> p, q and r are colinear 
	// 1 --> Clockwise 
	// 2 --> Counterclockwise 
	int orientation(Vertex v1, Vertex v2, Vertex v3);

	//initializes vertices before clipping by orienting input vertices
	//to change to clockwise orientation if counterclockwise originally
	void orientInitialVertices(int in, const Vertex inV[], Vertex oV[], bool cc);
	
	//sets each clipping boundary
	void setBoundaries(Vertex ll, Vertex ur);
	
	//returns if the point is inside the boundary
	//v- vertex we're looking at
	//boundary- value of the boundary (x or y)
	//bCase- which boundary are we looking at (top right left of bot)
	bool inside(Vertex v, float boundary, int bCase);

	// compute intersection along PS, put into i
	Vertex intersect(Vertex p, Vertex s, float boundary, int bCase);

	//debug function for printing vertex array
	void printOutV(Vertex arrayV[], int length);

	void tester(Vertex ll, Vertex ur);

};

#endif

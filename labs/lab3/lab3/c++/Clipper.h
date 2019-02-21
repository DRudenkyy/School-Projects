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

	//defining region codes 
	const int INSIDE = 0; // 0000 
	const int LEFT = 1;   // 0001 
	const int RIGHT = 2;  // 0010 
	const int BOTTOM = 4; // 0100 
	const int TOP = 8;    // 1000
	
	//defining clipping parameters
	int clip_xmin = 0;
	int clip_ymin = 0;
	int clip_xmax = 300;
	int clip_ymax = 300;
	
	std::list<Vertex> vertexList = {};

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
	
	//get the bitmasked code of a given vertex
	int computeCode(Vertex v);
	
	//set the new global clipping parameters
	void setNewClipParams(Vertex ll, Vertex ur);

	//debug function for printing vertex array
	void printOutV(Vertex arrayV[], int length);

	void tester(Vertex ll, Vertex ur);

};

#endif

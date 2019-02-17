//
//  Rasterizer.cpp
//
//  Created by Joe Geigel on 11/30/11.
//  Modifications by Warren R. Carithers.
//  Updated 09/21/2018 by wrc.
//  Copyright 2018 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Dmytro Rudenkyy
//

#include <list>
#include <algorithm>
#include <iostream>

#include "Rasterizer.h"
#include "Canvas.h"

using namespace std;


///
// Simple class that performs rasterization algorithms
///

///
// Constructor
//
// @param n number of scanlines
// @param C The Canvas to use
///
Rasterizer::Rasterizer( int n, Canvas &canvas ) : n_scanlines(n), C(canvas)
{
}

///
// Draw a filled polygon.
//
// Implementation should use the scan-line polygon fill algorithm
// discussed in class.
//
// The polygon has n distinct vertices.  The coordinates of the vertices
// making up the polygon are stored in the x and y arrays.  The ith
// vertex will have coordinate (x[i],y[i]).
//
// You are to add the implementation here using only calls to the
// setPixel() function.
//
// @param n - number of vertices
// @param x - array of x coordinates
// @param y - array of y coordinates
///
void Rasterizer::drawPolygon(int n, const int x[], const int y[] )
{
	//initialize edge table and active list
	initializeEdgeTable();
	activeEdgelist = nullptr;

	//loop through giiven vertices and prep the edgeTable
    for(int i = 0; i < n - 1; i = i + 2)
    {
		int yMin, yMax;
        int x0 = x[i];
		int y0 = y[i];
		int x1 = x[i + 1];
		int y1 = y[i + 1];
		if(y0 > y1)
		{
			yMin = y1;
			yMax = y0;
		}
		else
		{
			yMin = y0;
			yMax = y1;
		}

		int im = calcInverseSlope;
		if(im != 0)
		{
			//needs work here
			edgeTable[yMin]
		}
			
    }
}

float Rasterizer::calcInverseSlope(int x0, int y0, int x1, int y1)
{
	//line is horizontal, which we can ignore
	if(y0 == y1 || x0 == x1)
		return 0;
	
	return( (x0 - x1) / (y0 - y1) );

}

void Rasterizer::initializeEdgeTable()
{
    //set rows initially to empty
    for(int i = 0; i < n_scanlines; i++)
	edgeTable[i] = nullptr;
}


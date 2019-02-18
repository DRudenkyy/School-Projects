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

void Rasterizer::test()
{
	int x[] = {2, 7, 13, 13, 12, 10, 7, 2};
	int y[] = {9, 7, 11, 5, 1, 3, 1, 3};
	//int x[] = {2, 7, 13, 13, 7, 2};
	//int y[] = {9, 7, 11, 5, 1, 3};
	drawPolygon(8, x, y);
	printEdgeTable();
	
	EdgeBucket* bucketList = edgeTable[1];
	sortEdgeBuckets(bucketList);
	printEdgeTable();
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
	initializeEdgeTable();
	allocateEdgeTable(n, x, y);
	sortEdgeTable();
}

//initialize edge table
void Rasterizer::initializeEdgeTable()
{
    //set rows initially to empty
    for(int i = 0; i < n_scanlines; i++)
		edgeTable[i] = nullptr;
}

//sort the edgeTable
void Rasterizer::sortEdgeTable()
{
	for(int i = 0; i < n_scanlines; i++)
	{
		EdgeBucket* bucketList = edgeTable[i];
		if(bucketList != nullptr)
		{
			//only sort if there are 2 or more buckets per array
			if(bucketList->nextEdge != nullptr)
			{
				sortEdgeBuckets(bucketList);
			}
		}
	}
}

//sorts edge bucket lists by x value then inverse slope 
EdgeBucket* Rasterizer::sortEdgeBuckets(EdgeBucket* head) 
{
    EdgeBucket * curr;
    for(bool didSwap = true; didSwap; ) 
    {
        didSwap = false;
        for(curr = head; curr->nextEdge != nullptr; curr = curr->nextEdge) 
        {
			if ( (curr->x > curr->nextEdge->x)	//first sort on x 
            || ( (curr->x == curr->nextEdge->x) && //if xs are equal, sort by inverseSlope
				 (curr->inverseSlope > curr->nextEdge->inverseSlope) ) ) 
            {
				swapBucketValues(curr);
				didSwap = true;
			}			 
		}
    }
    return head;
}

//helper function used to swap values inside buckets
EdgeBucket* Rasterizer::swapBucketValues(EdgeBucket* bucket)
{
	EdgeBucket* nextBucket = bucket->nextEdge;
	int yMax = nextBucket->yMax;
	int initialX = nextBucket->x;
	int invM = nextBucket->inverseSlope;
	
	nextBucket->yMax = bucket->yMax;
	nextBucket->x = bucket->x;
	nextBucket->inverseSlope = bucket->inverseSlope;
	
	bucket->yMax = yMax;
	bucket->x = initialX;
	bucket->inverseSlope = invM;
	
	return bucket;
}

//loop through given vertices and prep the edgeTable
void Rasterizer::allocateEdgeTable(int n, const int x[], const int y[])
{
	for(int i = 0; i < n; i ++)
    {
		int yMin, yMax;
        int x0 = x[i];
		int y0 = y[i];
		
		int x1 = -1;
		int y1 = -1;
	    int initialX = -1;

		if(i + 1 != n) {
			x1 = x[i + 1];
			y1 = y[i + 1];
		}else {	//we are on the last point and looping back to start
			x1 = x[0];
			y1 = y[0];
		}//determine y Min and Max and where x starts
		if(y0 > y1) {
			yMin = y1;
			yMax = y0;
			initialX = x1;
		} else {
			yMin = y0;
			yMax = y1;
			initialX = x0;
		}
		//if the edge isn't a horizontal line we add it to ET
		if(y0 != y1)
		{
			float invM = (float)(x0 - x1) / (float)(y0 - y1);
			
			EdgeBucket* newEdge = new EdgeBucket();
			newEdge->yMax = yMax;
			newEdge->x = initialX;
			newEdge->inverseSlope = invM;
			newEdge->nextEdge = nullptr;
			//first edge being added to that y position
			if(edgeTable[yMin] == nullptr)
				edgeTable[yMin] = newEdge;
			else
			{	//go to end of linked edges and add new one at the end
				EdgeBucket *existingEdge = edgeTable[yMin];
				while(existingEdge->nextEdge != nullptr)
					existingEdge = existingEdge->nextEdge;
				existingEdge->nextEdge = newEdge;
			}
		}
	}
}

//debugging function
void Rasterizer::printEdgeTable()
{
	for(int i = 0; i < 12; i++)
	{
		cerr << i << ": |";
		EdgeBucket* bucketList = edgeTable[i];
		if(bucketList != nullptr)
		{
			do
			{
				cerr << bucketList->yMax << " | "  
				<< bucketList->x << " | "
				<< bucketList->inverseSlope << " | --> || ";
				bucketList = bucketList->nextEdge;
			}while(bucketList != nullptr);
		}
		else
			cerr << "NULL |";
			
		cerr << endl;
	}
}


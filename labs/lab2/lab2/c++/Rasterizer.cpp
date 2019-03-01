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
#include <math.h> 

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
	//int x[] = {2, 7, 13, 13, 12, 10, 7, 2};
	//int y[] = {9, 7, 11, 5, 1, 3, 1, 3};
	int x[] = {2, 7, 13, 13, 7, 2};
	//int y[] = {9, 7, 11, 5, 1, 3};
	int y[] = {3, 1, 5, 11, 7, 9};
	//drawPolygon(8, x, y);
	initializeEdgeTable();
	allocateEdgeTable(6, x, y);
	//sortEdgeTable();
	//printEdgeTable();
	//cerr << endl;
	processScanLines();
	//printEdgeTable();
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
	/*
	initializeEdgeTable();
	allocateEdgeTable(n, x, y);
	sortEdgeTable();
	printEdgeTable();
	processScanLines();*/
	
}

void Rasterizer::processScanLines()
{
	//Set y to the index of the first non-empty bucket list
	int firstEdgeYval = 0;
	while(edgeTable[firstEdgeYval] == nullptr)
		firstEdgeYval++;
		
	//go through all y pixels on the screen and process them
	//for(int y = firstEdgeYval; y < n_scanlines; y++)
	for(int y = firstEdgeYval; y < 12; y++)
	{
		//cerr << "ActiveList at " << y << ": "; printAL();
		if(edgeTable[y] != nullptr)
		{
			transferETBucketToAL(y);
			printEdgeTable();
		}
		
		if(activeEdgeList != nullptr)
		{
			cerr << "ActiveList ins't empty" << endl;
			discardYMaxEdges(y);
			//the last yMax was discarded and we can stop
			if(activeEdgeList == nullptr)
				return;
		}
		sortEdgeBuckets(activeEdgeList);
		//draw here?
		applySlope();
	}
}

//For each non-vertical edge in AL add 1/m to x
void Rasterizer::applySlope()
{
	EdgeBucket* curr;
	for(curr = activeEdgeList; curr != nullptr;) 
	{
		curr->x += curr->inverseSlope;
		curr = curr->nextEdge;
	}
}

//Fill pixels on scan line y using pairs of x coordinates from AL
void Rasterizer::drawScanLine(int y)
{
	float x1 = -1.0, x2 = -1.0;
	EdgeBucket* curr;
	for(curr = activeEdgeList; curr != nullptr;) 
	{
		if(x1 == -1.0)
			x1 = activeEdgeList->x;	//set the inside x
		else if(x2 == -1.0)
			x2 = activeEdgeList->x;	//set the outside x
			
		if(x1 != -1.0 && x2 != -1.0)
		{
			for(int i = (int)ceil(x1); i < (int)ceil(x2); i ++)
				C.setPixel(i, y);
				
			//reset these for the next two pairs	
			x1 = -1.0, x2 = -1.0;
		}
		curr = curr->nextEdge;
	}
}

//Move all buckets from ET[y] to AL
void Rasterizer::transferETBucketToAL(int currentY)
{
	//first time adding to AL
	if(activeEdgeList == nullptr)
		activeEdgeList = edgeTable[currentY];
	else
	{//find last edgeBucket in AL and append next ET[y]
		EdgeBucket *existingEdge = activeEdgeList;
		while(existingEdge->nextEdge != nullptr)
			existingEdge = existingEdge->nextEdge;
		existingEdge->nextEdge = edgeTable[currentY];
	}
	//dereference the buckets added to AL from ET
	edgeTable[currentY] = nullptr;
}

void Rasterizer::discardYMaxEdges(int currentY)
{
	cerr << "before" << endl;
	printAL();
	//the ActiveList has only one bucket
	EdgeBucket* curr;
	for(curr = activeEdgeList; curr != nullptr;) 
	{
		if(curr->yMax == currentY)
		{
			cerr << currentY << " discarding " << activeEdgeList->yMax << " | "  
			<< activeEdgeList->x << " | "
			<< activeEdgeList->inverseSlope << " |";
			curr = curr->nextEdge;
			//delete activeEdgeList;
			activeEdgeList = curr;
		}
		else {
			cerr << "next" << endl;
			curr = curr->nextEdge;
		}
	}
	cerr << "after" << endl;
	printAL();
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
			sortEdgeBuckets(bucketList);
		}
	}
}

//sorts edge bucket lists by x value then inverse slope 
EdgeBucket* Rasterizer::sortEdgeBuckets(EdgeBucket* head) 
{
	//don't sort if bucket is empty or only has one edge
	if(head == nullptr || head->nextEdge == nullptr)
		return head;
		
    EdgeBucket * curr;
    for(bool didSwap = true; didSwap;) 
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
	cerr << "EdgeTable" << endl; 
	for(int i = 11; i >= 0; i--)
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

//debugging function
void Rasterizer::printAL()
{
		//cerr << "ActiveList" << endl;
		if(activeEdgeList != nullptr)
		{
			if(activeEdgeList->nextEdge != nullptr)
			{
				do
				{
					cerr << activeEdgeList->yMax << " | "  
					<< activeEdgeList->x << " | "
					<< activeEdgeList->inverseSlope << " | --> || ";
					activeEdgeList = activeEdgeList->nextEdge;
				}while(activeEdgeList != nullptr);
			}else
			{
				cerr << activeEdgeList->yMax << " | "  
					<< activeEdgeList->x << " | "
					<< activeEdgeList->inverseSlope << " | --> || ";
					activeEdgeList = activeEdgeList->nextEdge;
			}
		}
		cerr << endl;
}


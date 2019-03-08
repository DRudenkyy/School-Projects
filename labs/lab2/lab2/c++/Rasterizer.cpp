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
	processScanLines();
}

//the main function that iterates through every scanline of a polygon,
//from its min to max yval
void Rasterizer::processScanLines()
{
	//Set y to the index of the first non-empty bucket list
	int firstEdgeYval = 0;
	while(edgeTable[firstEdgeYval] == nullptr)
		firstEdgeYval++;
		
	//go through all y pixels on the screen and process them
	for(int y = firstEdgeYval; y < n_scanlines; y++)
	{		
		//drop from activeList if necessary
		if(activeEdgeList != nullptr)
			discardYMaxEdges(y);
		//move edgeBucket into AL
		if(edgeTable[y] != nullptr)
			transferETBucketToAL(y);
		//the last yMax was discarded and we can stop
		if(activeEdgeList == nullptr)
			return;
			
		sortEdgeBuckets(activeEdgeList);
		drawScanLine(y);
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
	int pixMin = -1, pixMax = -1;
	EdgeBucket* curr;
	for(curr = activeEdgeList; curr != nullptr;) 
	{
		if(x1 == -1.0)
			x1 = curr->x;	//set the inside x
		else
			x2 = curr->x;	//set the outside x
			
		if(x1 != -1.0 && x2 != -1.0)
		{
			pixMin = (int)ceil(x1);
			pixMax = (int)ceil(x2);
			if(pixMin == pixMax)
			{
				//cerr << "This is an edge pixel, do not draw" << endl;
			}
			else
			{
				for(int i = pixMin; i < pixMax; i ++)
					C.setPixel(i, y);
			}
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

//discard and delete edgeBuckets from AL that have already been processed
void Rasterizer::discardYMaxEdges(int currentY)
{
	EdgeBucket* curr;
	EdgeBucket* prev;
	for(curr = activeEdgeList; curr != nullptr;) 
	{		
		if(curr->yMax == currentY)
		{
			if(curr == activeEdgeList)	//deleting head
				activeEdgeList = curr->nextEdge;
			else
				prev->nextEdge = curr->nextEdge;
				
			delete curr;	//deallocate memory
		}
		else {
			prev = curr;
			curr = curr->nextEdge;
		}
	}
}

//initialize edge table
void Rasterizer::initializeEdgeTable()
{
    //set rows initially to empty
    for(int i = 0; i < n_scanlines; i++)
		edgeTable[i] = nullptr;
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
	float initialX = nextBucket->x;
	float invM = nextBucket->inverseSlope;
	
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
	for(int i = 220; i >= 110; i--)
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
		cerr << "ActiveList" << endl;
		EdgeBucket* curr =  activeEdgeList;
		if(curr != nullptr)
		{
			if(curr->nextEdge != nullptr)
			{
				do
				{
					cerr << curr->yMax << " | "  
					<< curr->x << " | "
					<< curr->inverseSlope << " | --> || ";
					curr = curr->nextEdge;
				}while(curr != nullptr);
			}else
			{
				cerr << curr->yMax << " | "  
					<< curr->x << " | "
					<< curr->inverseSlope << " | --> || ";
					curr = curr->nextEdge;
			}
		}
		cerr << endl;
}


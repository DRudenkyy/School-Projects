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
	int x[] = {2, 7, 13, 13, 7, 2};
	int y[] = {3, 1, 5, 11, 7, 9};
	drawPolygon(6, x, y);
	allocateEdgeTable(6, x, y);
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
	//initialize edge table
	initializeEdgeTable();
	//allocateEdgeTable(n, x, y);
	//sortEdgeTable();
}

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

//initialize edge table
void Rasterizer::initializeEdgeTable()
{
    //set rows initially to empty
    for(int i = 0; i < n_scanlines; i++)
		edgeTable[i] = nullptr;
}
/*
//sort the edgeTable
void Rasterizer::sortEdgeTable()
{
	for(int i = 0; i < n_scanlines; i++)
	{
		EdgeBucket* bucketList = edgeTable[i];
		if(bucketList != nullptr)
		{
			if(bucketList->nextEdge != nullptr)
			{
				EdgeBucket* sortedBucketList = sortEdgeBuckets(bucketList);
				delete(bucketList);
				edgeTable[i] = sortedBucketList;
			}
		}
	}
}

EdgeBucket* Rasterizer::sortEdgeBuckets(EdgeBucket* head) 
{
    EdgeBucket *top = nullptr; // first EdgeBucket we will return this value
    EdgeBucket *current = nullptr;
    bool sorted = false;
    while (sorted == false) 
    {
        // we are going to look for the lowest value in the list
        EdgeBucket *parent = head;
        EdgeBucket *lowparent = head; // we need this because list is only linked forward
        EdgeBucket *low = head; // this will end up with the lowest EdgeBucket
        sorted = true;
        do 
        {
            // find the lowest valued key
            EdgeBucket* next = parent->nextEdge;
            if ( (parent->x > next->x)	//first sort on x 
            || ( (parent->x == next->x) && //if xs are equal, sort by inverseSlope
				 (parent->inverseSlope > next->inverseSlope) ) ) 
            {
                lowparent = parent;
                low = next;
                sorted = false;
			}
            parent = parent->nextEdge;
		} while (parent->nextEdge != nullptr);
		
        if (current != nullptr) // first time current == nullptr
            current->nextEdge = low;
	
        // remove the lowest item from the list and reconnect the list
        // basically you are forming two lists, one with the sorted EdgeBuckets 
        // and one with the remaining unsorted EdgeBuckets
        current = low;
        if (current == head) { head = current->nextEdge; }
        lowparent->nextEdge = low->nextEdge;
        current->nextEdge = nullptr;
        if (top == nullptr) {
            top = current;
            }
        };
    current->nextEdge = head;
    return top;
}
*/
//loop through given vertices and prep the edgeTable
void Rasterizer::allocateEdgeTable(int n, const int x[], const int y[])
{
	for(int i = 0; i < n; i ++)
    {
		cerr << " ADDING ";
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
			
			if(edgeTable[yMin] == nullptr)
				edgeTable[yMin] = newEdge;
			else
			{
				EdgeBucket *existingEdge = edgeTable[yMin];
				while(existingEdge->nextEdge != nullptr)
					existingEdge = existingEdge->nextEdge;
				existingEdge->nextEdge = newEdge;
			}
		}
	}
}

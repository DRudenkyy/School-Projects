//
//  Pipeline.cpp
//
//  Created by Warren R. Carithers on 2016/10/19.
//  Based on a version created by Joe Geigel on 11/30/11.
//  Updated 2018/10/12 by wrc.
//  Copyright 2018 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Dmytro Rudenkyy
//

#include <iostream>
#include "Pipeline.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

///
// Simple wrapper class for midterm assignment
//
// Only methods in the implementation file whose bodies
// contain the comment
//
//    // YOUR IMPLEMENTATION HERE
//
// are to be modified by students.
///

///
// Constructor
//
// @param w width of canvas
// @param h height of canvas
///
Pipeline::Pipeline( int w, int h ) : Canvas(w,h)
    // YOUR IMPLEMENTATION HERE if you need to add initializers
{
    // YOUR IMPLEMENTATION HERE if you need to modify the constructor
}

///
// addPoly - Add a polygon to the canvas.  This method does not draw
//           the polygon, but merely stores it for later drawing.
//           Drawing is initiated by the drawPoly() method.
//
//           Returns a unique integer id for the polygon.
//
// @param p - Array containing the vertices of the polygon to be added.
// @param n - Number of vertices in polygon
//
// @return a unique integer identifier for the polygon
///
int Pipeline::addPoly( const Vertex p[], int n )
{
    Polygon* newPol = new Polygon();
    
    bool clockwise = true;
	
	if(orientation(p[0], p[1], p[2]) == 2)
		clockwise = false;
	else if(orientation(p[0], p[1], p[2]) == 0)
		cerr << "shouldn't happen";
	
	//this will orient the original vertices to get a clockwise orientation
	//necessary to do clipping properly but not for drawing
	orientInitialVertices(n, p, newPol->p, clockwise);
	
	newPol->n = n;
	polyArray[polyIdIndexer] = newPol;
	polyIdIndexer++;

    return polyIdIndexer - 1;
}

///
// drawPoly - Draw the polygon with the given id.  The polygon should
//            be drawn after applying the current transformation to
//            the vertices of the polygon.
//
// @param polyID - the ID of the polygon to be drawn.
///
void Pipeline::drawPoly( int polyID )
{
	Polygon* thisPoly = polyArray[polyID];

	Vertex outV[50];
	int n = clipPolygon(thisPoly, outV);
	

	
	initializeEdgeTable();
	allocateEdgeTable(n, outV);
	processScanLines();
}

void Pipeline::genWorldToViewMatrix()
{
	//boundaries
	//0    1    2      3
	//top right bottom left
	
	//viewport
	//0 1 2     3
	//x y width height
	
	float xdMin = viewPort[0];
	float xdMax = xdMin + viewPort[2];
	float ydMin = viewPort[1];
	float ydMax = ydMin + viewPort[3];
	
	float topL = float( (xdMax - xdMin) / (boundaries[1] - boundaries[3]) );
	
	float topR = float( (boundaries[1] * xdMin - boundaries[3] * xdMax)
					  /	(boundaries[1] - boundaries[3]) );
					  
	float mid = float( (ydMax - ydMin) / (boundaries[0] - boundaries[2]) );
	
	float midR = float( (boundaries[0] * ydMin - boundaries[2] * ydMax)
					  /	(boundaries[1] - boundaries[3]) );
					  
	worldToViewMatrix = glm::mat3(topL, 0.0, topR,
								  0.0, mid, midR,
								  0.0, 0.0, 1.0);
}

///
// clearTransform - Set the current transformation to the identity matrix.
///
void Pipeline::clearTransform( void )
{
    transformMatrix = glm::mat3(1.0, 0.0, 0.0,
								0.0, 1.0, 0.0,
								0.0, 0.0, 1.0);
}

///
// translate - Add a translation to the current transformation by
//             premultiplying the appropriate translation matrix to
//             the current transformation matrix.
//
// @param x - Amount of translation in x.
// @param y - Amount of translation in y.
///
void Pipeline::translate( float tx, float ty )
{
    glm::mat3 translationMatrix = glm::mat3(1.0, 0.0, tx,
											0.0, 1.0, ty,
											0.0, 0.0, 1.0);
											
	transformMatrix = transformMatrix * translationMatrix;
}

///
// rotate - Add a rotation to the current transformation by premultiplying
//          the appropriate rotation matrix to the current transformation
//          matrix.
//
// @param degrees - Amount of rotation in degrees.
///
void Pipeline::rotate( float degrees )
{
	float radians = glm::radians(degrees);
    glm::mat3 rotationMatrix = glm::mat3(cos(radians), -sin(radians), 0.0,
										 sin(radians), cos(radians), 0.0,
										 0.0, 			0.0, 		1.0);
										 
	transformMatrix = rotationMatrix * transformMatrix;
}

///
// scale - Add a scale to the current transformation by premultiplying
//         the appropriate scaling matrix to the current transformation
//         matrix.
//
// @param x - Amount of scaling in x.
// @param y - Amount of scaling in y.
///
void Pipeline::scale( float sx, float sy )
{
    glm::mat3 scaleMatrix = glm::mat3(sx, 0.0, 0.0, 
									  0.0, sy, 0.0, 
									  0.0, 0.0, 1.0);
									  
	transformMatrix = transformMatrix * scaleMatrix;
}

///
// setClipWindow - Define the clip window.
//
// @param bottom - y coord of bottom edge of clip window (in world coords)
// @param top - y coord of top edge of clip window (in world coords)
// @param left - x coord of left edge of clip window (in world coords)
// @param right - x coord of right edge of clip window (in world coords)
///
void Pipeline::setClipWindow( float bottom, float top, float left, float right )
{
    boundaries[0] = top;	//top
	boundaries[1] = right;	//right
	boundaries[2] = bottom;	//bottom
	boundaries[3] = left;	//left
}

///
// setViewport - Define the viewport.
//
// @param xmin - x coord of lower left of view window (in screen coords)
// @param ymin - y coord of lower left of view window (in screen coords)
// @param width - width of view window (in world coords)
// @param height - width of view window (in world coords)
///
void Pipeline::setViewport( int x, int y, int width, int height )
{
    viewPort[0] = x;
    viewPort[1] = y;
    viewPort[2] = width;
    viewPort[3] = height;
}


//Clipping Related Functions--------------------------------------------

int Pipeline::clipPolygon(Polygon* poly, Vertex outV[])
{
	//create a temporary copy of polygon vertices to preserve original data
	Vertex temp[50];
	int in = poly->n;
	for(int i = 0; i < in; i++)
		temp[i] = poly->p[i];
	
	int outLength = 0;
	int bCase = 0;	//stands for which boundary we are clipping against
	while(bCase < 4)
	{
		outLength = 0;   Vertex p = temp[in - 1]; 
		for (int j = 0; j < in; j++) {
			Vertex s = temp[j];

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
					temp[i] = outV[i];
			}
			
			bCase ++;
	}
	
    return(outLength);  // remember to return the outgoing vertex count!
}

// To find orientation of ordered triplet (v1, v2, v3). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int Pipeline::orientation(Vertex v1, Vertex v2, Vertex v3) 
{ 
    int val = (v2.y - v1.y) * (v3.x - v2.x) - 
              (v2.x - v1.x) * (v3.y - v2.y); 
  
    if (val == 0) return 0;  // colinear 
  
    return (val > 0)? 1: 2; // clock or counterclock wise 
}

//initializes vertices before clipping by orienting input vertices
//to change to clockwise orientation if counterclockwise originally
void Pipeline::orientInitialVertices(int in, const Vertex inV[], 
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

// compute intersection along PS, return Vertex i
Vertex Pipeline::intersect(Vertex p, Vertex s, float boundary, int bCase)
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
bool Pipeline::inside(Vertex v, float boundary, int bCase)
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

//PolyDrawing Related Functions-----------------------------------------

//the main function that iterates through every scanline of a polygon,
//from its min to max yval
void Pipeline::processScanLines()
{
	//Set y to the index of the first non-empty bucket list
	int firstEdgeYval = 0;
	while(edgeTable[firstEdgeYval] == nullptr)
		firstEdgeYval++;
		
	//go through all y pixels on the screen and process them
	for(int y = firstEdgeYval; y < MAX_SCANLINES; y++)
	{		
		cerr << y << endl;
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
void Pipeline::applySlope()
{
	EdgeBucket* curr;
	for(curr = activeEdgeList; curr != nullptr;) 
	{
		curr->x += curr->inverseSlope;
		curr = curr->nextEdge;
	}
}

//Fill pixels on scan line y using pairs of x coordinates from AL
void Pipeline::drawScanLine(int y)
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
					setPixel(i, y);
			}
			//reset these for the next two pairs	
			x1 = -1.0, x2 = -1.0;
		}
		curr = curr->nextEdge;
	}
}

//Move all buckets from ET[y] to AL
void Pipeline::transferETBucketToAL(int currentY)
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
void Pipeline::discardYMaxEdges(int currentY)
{
	EdgeBucket* curr;
	EdgeBucket* prev;
	for(curr = activeEdgeList; curr != nullptr;) 
	{		
		if(curr->yMax == currentY)
		{
			if(curr == activeEdgeList)	//deleting head
			{
				activeEdgeList = curr->nextEdge;	//move the head over
				delete curr;	//deallocate memory
				curr = activeEdgeList;	//set the next curr
			}
			else   //deleting middle or tail
			{
				prev->nextEdge = curr->nextEdge;	//set the prev
				delete curr;	//deallocate memory
				curr = prev->nextEdge;	//set the next curr
			}			
		}
		else {
			prev = curr;
			curr = curr->nextEdge;
		}
	}
}

//initialize edge table
void Pipeline::initializeEdgeTable()
{
    //set rows initially to empty
    for(int i = 0; i < MAX_SCANLINES; i++)
		edgeTable[i] = nullptr;
}

//sorts edge bucket lists by x value then inverse slope 
EdgeBucket* Pipeline::sortEdgeBuckets(EdgeBucket* head) 
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
EdgeBucket* Pipeline::swapBucketValues(EdgeBucket* bucket)
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
void Pipeline::allocateEdgeTable(int n, Vertex v[])
{
	cerr << "STRT";
	for(int i = 0; i < n; i ++)
    {
		float yMin, yMax;
        float x0 = v[i].x;
		float y0 = v[i].y;
		
		float x1 = -1;
		float y1 = -1;
	    float initialX = -1;

		if(i + 1 != n) {
			x1 = v[i + 1].x;
			y1 = v[i + 1].y;
		}else {	//we are on the last point and looping back to start
			x1 = v[0].x;
			y1 = v[0].y;
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
			if(edgeTable[(int)yMin] == nullptr)
				edgeTable[(int)yMin] = newEdge;
			else
			{	//go to end of linked edges and add new one at the end
				EdgeBucket *existingEdge = edgeTable[(int)yMin];
				while(existingEdge->nextEdge != nullptr)
					existingEdge = existingEdge->nextEdge;
				existingEdge->nextEdge = newEdge;
			}
		}
	}
	cerr << "END" << endl;
}

//debugging function
void Pipeline::printEdgeTable()
{
	cerr << "EdgeTable" << endl; 
	for(int i = 190; i >= 10; i--)
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
void Pipeline::printAL()
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

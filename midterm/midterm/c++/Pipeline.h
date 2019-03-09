//
//  Pipeline.h
//
//  Created by Warren R. Carithers on 2016/10/19.
//  Based on a version created by Joe Geigel on 11/30/11.
//  Updated 2018/10/12 by wrc.
//  Copyright 2018 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Dmytro Rudenkyy
//

#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include "Canvas.h"
#include "Vertex.h"

using namespace std;

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

struct Polygon {
	int n; //number of vertices
	Vertex p[50];	//Array containing the vertices of the polygon to be added.
};

struct EdgeBucket {
    int   yMax;         // Final Y
    float x;            // Initial X
    float inverseSlope; // Slope of edge WRT Y
    EdgeBucket* nextEdge;     // Reference to next Edge at scanline
};

class Pipeline : public Canvas {

public:
    Polygon* polyArray[25];
    int polyIdIndexer = 0;
    
    //clipping boundaries. x value (if right or left boundary) or y value (otherwise)
	//and when to check vertex < boundary (inside for right and top)
	//and vertex > boundary (inside for left and bottom).
	float boundaries[4];	//top right bottom left
    
    //We are guaranteed a 500-pixel vertical resolution
	//I am using this as a crutch since I spent way too much time 
	//trying to figure out how to dynamically allocate array sizes in C (using malloc)
    static const int MAX_SCANLINES = 500;

    // Each scanline has a pointer to its first Edge
    EdgeBucket* edgeTable[MAX_SCANLINES];

    EdgeBucket* activeEdgeList = nullptr;    // No edges until we hit
											// first scanline with edges.
    ///
    // Constructor
    //
    // @param w width of canvas
    // @param h height of canvas
    ///
    Pipeline(int w, int h);
    


    ///
    // addPoly - Add a polygon to the canvas.  This method does not draw
    //           the polygon, but merely stores it for later drawing.
    //           Drawing is initiated by the drawPoly() method.
    //
    // @param p - Array of Vertex entries defining the polygon to be added
    // @param n - Number of vertices in polygon
    //
    // @return a unique integer identifier for the polygon
    ///
    int addPoly( const Vertex p[], int n );

    ///
    // drawPoly - Draw the polygon with the given id.  The polygon should
    //            be drawn after applying the current transformation to
    //            the vertices of the polygon.
    //
    // @param polyID - the ID of the polygon to be drawn.
    ///
    void drawPoly( int polyID );

    ///
    //
    // clearTransform - Set the current transformation to the identity matrix.
    //
    ///
    void clearTransform( void );

    ///
    // translate - Add a translation to the current transformation by
    //             premultiplying the appropriate translation matrix to
    //             the current transformation matrix.
    //
    // @param tx - Amount of translation in x.
    // @param ty - Amount of translation in y.
    //
    ///
    void translate( float tx, float ty );

    ///
    // rotate - Add a rotation to the current transformation by premultiplying
    //          the appropriate rotation matrix to the current transformation
    //          matrix.
    //
    // @param degrees - Amount of rotation in degrees.
    ///
    void rotate( float degrees );

    ///
    // scale - Add a scale to the current transformation by premultiplying
    //         the appropriate scaling matrix to the current transformation
    //         matrix.
    //
    // @param sx - Amount of scaling in x.
    // @param sy - Amount of scaling in y.
    ///
    void scale( float sx, float sy );

    ///
    // setClipWindow - Define the clip window.
    //
    // @param bottom - y coord of bottom edge of clip window (in world coords)
    // @param top - y coord of top edge of clip window (in world coords)
    // @param left - x coord of left edge of clip window (in world coords)
    // @param right - x coord of right edge of clip window (in world coords)
    ///
    void setClipWindow( float bottom, float top, float left, float right );

    ///
    // setViewport - Define the viewport.
    //
    // @param xmin - x coord of lower left of view window (in screen coords)
    // @param ymin - y coord of lower left of view window (in screen coords)
    // @param width - width of view window (in world coords)
    // @param height - width of view window (in world coords)
    ///
    void setViewport( int x, int y, int width, int height );


	//Clipping Related Functions--------------------------------------------
	int clipPolygon(Polygon* poly, Vertex outV[]);
	
	// To find orientation of ordered triplet (v1, v2, v3). 
	// The function returns following values 
	// 0 --> p, q and r are colinear 
	// 1 --> Clockwise 
	// 2 --> Counterclockwise 
	int orientation(Vertex v1, Vertex v2, Vertex v3);
	
	//initializes vertices before clipping by orienting input vertices
	//to change to clockwise orientation if counterclockwise originally
	void orientInitialVertices(int in, const Vertex inV[], 
												Vertex oV[], bool cc);
												
    //returns if the point is inside the boundary
	//v- vertex we're looking at
	//boundary- value of the boundary (x or y)
	//bCase- which boundary are we looking at (top right left of bot)
	bool inside(Vertex v, float boundary, int bCase);

	// compute intersection along PS, put into i
	Vertex intersect(Vertex p, Vertex s, float boundary, int bCase);
	
	//PolyDrawing Related Functions-----------------------------------------
	void drawPolygon(int n, Vertex v[]);

    void initializeEdgeTable();
    
    void allocateEdgeTable(int n, Vertex v[]);
        
    EdgeBucket* sortEdgeBuckets(EdgeBucket* head);
    
    EdgeBucket* swapBucketValues(EdgeBucket* bucket);
    
    void processScanLines();
    
    void drawScanLine(int y);
    
    void discardYMaxEdges(int currentY);
    
    void transferETBucketToAL(int currentY);
    
    void applySlope();
    
    void printEdgeTable();
    void printAL();
};

#endif

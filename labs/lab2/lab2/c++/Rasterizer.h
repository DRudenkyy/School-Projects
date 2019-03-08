//
//  Rasterizer.h
//
//  Created by Joe Geigel on 11/30/11.
//  Modifications by Warren R. Carithers.
//  Updated 09/21/2018 by wrc.
//  Copyright 2018 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Dmytro Rudenkyy
//

#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

class Canvas;

struct EdgeBucket {
    int   yMax;         // Final Y
    float x;            // Initial X
    float inverseSlope; // Slope of edge WRT Y
    EdgeBucket* nextEdge;     // Reference to next Edge at scanline
};

///
// Simple class that performs rasterization algorithms
///

class Rasterizer {

    ///
    // number of scanlines
    ///

    int n_scanlines;

	//We are guaranteed a 600-pixel vertical resolution
	//I am using this as a crutch since I spent way too much time 
	//trying to figure out how to dynamically allocate array sizes in C (using malloc)
    static const int MAX_SCANLINES = 600;

    // Each scanline has a pointer to its first Edge
    EdgeBucket* edgeTable[MAX_SCANLINES];

    EdgeBucket* activeEdgeList = nullptr;    // No edges until we hit
											// first scanline with edges.
    
public:


    ///
    // Drawing canvas
    ///

    Canvas &C;

    ///
    // Constructor
    //
    // @param n number of scanlines
    // @param C The Canvas to use
    ///
    Rasterizer( int n, Canvas &canvas );
    
    ///
    // Draw a filled polygon
    //
    // Implementation should use the scan-line polygon fill algorithm
    // discussed in class.
    //
    // The polygon has n distinct vertices.  The coordinates of the vertices
    // making up the polygon are stored in the x and y arrays.  The ith
    // vertex will have coordinate (x[i],y[i]).
    //
    // You are to add the implementation here using only calls to the
    // setPixel() method of the canvas.
    //
    // @param n - number of vertices
    // @param x - array of x coordinates
    // @param y - array of y coordinates
    ///
    void drawPolygon( int n, const int x[], const int y[] );

    void initializeEdgeTable();
    
    void allocateEdgeTable(int n, const int x[], const int y[]);
        
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

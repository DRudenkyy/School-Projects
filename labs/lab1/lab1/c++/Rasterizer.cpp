//
//  Rasterizer.cpp
//
//  Created by Warren R. Carithers on 01/28/14.
//  Based on a C++ version created by Joe Geigel on 11/30/11.
//  Copyright 2014 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Dmytro Rudenkyy
//

#include <cmath>

#include "Rasterizer.h"

///
//
// Simple class that performs rasterization algorithms
//
///

///
// Constructor
//
// @param n number of scanlines
// @param C the Canvas to be used
///

Rasterizer::Rasterizer( int n, Canvas &canvas ) : n_scanlines(n), C(canvas)
{
}


///
// Draw my initials
//
// Draw my own initials using calls to drawLine(), in the same
// manner that makeLines() in the driver program draws 'CG'.
///

void Rasterizer::myInitials( void ) {

    // ######## Use light blue (0.678,0.847,0.902) for initials ######## 

    C.setColor( 0.678, 0.847, 0.902 );

    drawLine(0, 0, 600, 600);
    drawLine(0, 600, 600, 5);
    drawLine(300, 0, 300, 600); 
    drawLine(100, 600, 100, 0); 




    //
    // add code here to draw your initials
    // with calls to your drawLine() function
    // 

}

///
// Draw a line from (x0,y0) to (x1, y1)
//
// Implementation should be using the Midpoint Line Algorithm.
//
// You are to add the implementation here using only calls to the
// setPixel() method of the Canvas.
//
// @param x0 x coord of first endpoint
// @param y0 y coord of first endpoint
// @param x1 x coord of second endpoint
// @param y1 y coord of second endpoint
///

void Rasterizer::drawLine( int x0, int y0, int x1, int y1 )
{
  if(x0 > x1)//line is being drawn right to left so flip
    {
      int tempx0 = x0;
      int tempy0 = y0;
      x0 = x1;
      y0 = y1;
      x1 = tempx0;
      y1 = tempy0;
    }

  bool negativeSlope = false;

  if(y0 > y1)
    negativeSlope = true;

  int dE, dSNE, x, y, d, m;
  int dy = y1 - y0;
  int dx = x1 - x0;

  if(x0 == x1)
  {
    if(y0 < y1)
    {
      for(y = y0; y <= y1; y++)
      {
	C.setPixel(x0,y);
      }
    }
    else
    {
      for(y = y1; y <= y0; y++)
      {
        C.setPixel(x0,y);
      }
    }
  }
  else if(abs(dx) > abs(dy)) //slope is 0 < m < 1
  {
    dE = 2 * dy;
    if(negativeSlope) {
      dSNE = 2 * (dx + dy);
      d = dE + dx;
    }
    else {
      dSNE = 2 * (dx - dy);
      d = dE - dy;
    }
    
    for(x = x0, y = y0; x <= x1; ++x)
    {
      C.setPixel(x, y);

      if(d <= 0) {
	if(negativeSlope)
	  d -= dE;
	else
	  d += dE;
      }
      else {
	if(negativeSlope) {
          --y;
	  d -= dSNE;
	}
	else {
	  ++y;
	  d += dSNE;
	}
      }
    }
  }
  else
  {
    dE = 2 * dy;
    if(negativeSlope) {
      dSNE = 2 * (dx + dy);
      d = dE + dx;
    }
    else {
      dSNE = 2 * (dx - dy);
      d = dE - dy;
    }
  
    for(x = x0, y = y0; y <= y1; ++y)
    {
      C.setPixel(x, y);
      
      if(d <= 0) {
	if(negativeSlope)
	  d -= dE;
	else
	  d += dE;
      }
      else {
	if(negativeSlope) {
          --x;
	  d -= dSNE;
	}
	else {
	  ++x;
	  d += dSNE;
	}
      }
    }
  }
}

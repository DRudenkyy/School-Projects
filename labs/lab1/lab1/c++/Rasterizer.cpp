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
  int dE, dNE, x, y, d, m;
  int dy = y1 - y0;
  int dx = x1 - x0;

  dE = 2 * dy;
  dNE = 2 * (dy - dx);
  d = dE - dx;

  if(abs(dx) >= abs(dy)) //slope is 0 < m <=1
  {
    for(x = x0, y = y0; x <= x1; ++x)
    {
	C.setPixel(x, y);

	if(d <= 0) {
	  d += dE;
	}
	else {
	  ++y;
	  d += dNE;
	}
    }
  }
  else
  {
    for(x = x0, y = y0; y <= y1; ++y)
    {
      C.setPixel(x, y);

      if(d <= 0) {
	  d += dE;
      }
      else {
        ++x;
        d += dNE;
      }
    }
  }
}

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

    //D
    drawLine(25, 25, 25, 250);//1
    drawLine(25, 250, 100, 250);//2
    drawLine(100, 250, 175, 100);//3
    drawLine(175, 150, 175, 100);//4
    drawLine(175, 100, 100, 25);//5
    drawLine(100, 25, 25, 25);//6
    //inside d
    drawLine(5+50, 5+100, 5+50, 50+100);//1
    drawLine(55, 150, 70, 150);//2
    drawLine(70, 150, 85, 120);//3
    drawLine(85, 130, 85, 115);//4
    drawLine(85, 115, 70, 105);//5
    drawLine(55, 105, 70, 105);//6

    //R
    drawLine(325, 25, 325, 250);//1
    drawLine(325, 250, 500, 250);//2
    drawLine(500, 250, 500, 175);//3
    drawLine(500, 175, 375, 175);//4
    drawLine(375, 175, 535, 25);//5
    drawLine(535, 25, 480, 25);//6
    drawLine(480, 25, 375, 125);//7
    drawLine(375, 125, 375, 25);//8
    drawLine(375, 25, 325, 25);//8


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

  if(x0 == x1)//vertical line
  {
    if(y0 < y1)//bottom to up
    {
      for(y = y0; y <= y1; y++)
      {
	C.setPixel(x0,y);
      }
    }
    else//top to bottom
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
  else//large slope
  {
    dE = 2 * dx;
    if(negativeSlope) {
      dSNE = 2 * (dx + dy);
      d = dE + dy;
    }
    else {
      dSNE = 2 * (dx - dy);
      d = dE - dy;
    }
  
    for(x = x0, y = y0; y <= y1; ++y)
    {
      C.setPixel(x, y);
      
      if(d <= 0)
      {
	if(negativeSlope)
	  d -= dE;
	else
	  d += dE;
      }
      else
      {
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

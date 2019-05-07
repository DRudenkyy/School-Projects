///
//  Shapes.cpp
//
//  Created by Warren R. Carithers 2018/04/19.
//  Based on earlier versions by Warren R. Carithers and Joe Geigel.
//  Copyright 2018 Rochester Institute of Technology.  All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
//
//  Routines for shape-drawing functions for the sphere.
//
//  This file should only be modified if you are replacing the Phong-
//  shaded version of the sphere with a texture-mapped version.
///

#include <iostream>
#include <cmath>
using namespace std;

#include "Canvas.h"
#include "Sphere.h"
#include "Lighting.h"
#include "Textures.h"
#include "Viewing.h"

/*
** The sphere
*/

//
// Each group of three values specifies a sphere vertex
//
// These values double as surface normals, because the sphere is
// centered at (0,0,0).
//
float sphereVertices[] = {
  0.000000f,  0.500000f,  0.000000f,  0.124968f,  0.475553f,  0.090733f,
  0.154433f,  0.475553f,  0.000000f,  0.000000f,  0.500000f,  0.000000f,
  0.047816f,  0.475553f,  0.146844f,  0.124968f,  0.475553f,  0.090733f,
  0.000000f,  0.500000f,  0.000000f, -0.047582f,  0.475553f,  0.146920f,
  0.047816f,  0.475553f,  0.146844f,  0.000000f,  0.500000f,  0.000000f,
 -0.124823f,  0.475553f,  0.090932f, -0.047582f,  0.475553f,  0.146920f,
  0.000000f,  0.500000f,  0.000000f, -0.154433f,  0.475553f,  0.000246f,
 -0.124823f,  0.475553f,  0.090932f,  0.000000f,  0.500000f,  0.000000f,
 -0.125112f,  0.475553f, -0.090534f, -0.154433f,  0.475553f,  0.000246f,
  0.000000f,  0.500000f,  0.000000f, -0.048050f,  0.475553f, -0.146768f,
 -0.125112f,  0.475553f, -0.090534f,  0.000000f,  0.500000f,  0.000000f,
  0.047348f,  0.475553f, -0.146995f, -0.048050f,  0.475553f, -0.146768f,
  0.000000f,  0.500000f,  0.000000f,  0.124678f,  0.475553f, -0.091131f,
  0.047348f,  0.475553f, -0.146995f,  0.000000f,  0.500000f,  0.000000f,
  0.154432f,  0.475553f, -0.000492f,  0.124678f,  0.475553f, -0.091131f,
  0.154433f,  0.475553f,  0.000000f,  0.124968f,  0.475553f,  0.090733f,
  0.293764f,  0.404602f,  0.000000f,  0.124968f,  0.475553f,  0.090733f,
  0.237715f,  0.404602f,  0.172594f,  0.293764f,  0.404602f,  0.000000f,
  0.124968f,  0.475553f,  0.090733f,  0.047816f,  0.475553f,  0.146844f,
  0.237715f,  0.404602f,  0.172594f,  0.047816f,  0.475553f,  0.146844f,
  0.090956f,  0.404602f,  0.279328f,  0.237715f,  0.404602f,  0.172594f,
  0.047816f,  0.475553f,  0.146844f, -0.047582f,  0.475553f,  0.146920f,
  0.090956f,  0.404602f,  0.279328f, -0.047582f,  0.475553f,  0.146920f,
 -0.090511f,  0.404602f,  0.279473f,  0.090956f,  0.404602f,  0.279328f,
 -0.047582f,  0.475553f,  0.146920f, -0.124823f,  0.475553f,  0.090932f,
 -0.090511f,  0.404602f,  0.279473f, -0.124823f,  0.475553f,  0.090932f,
 -0.237440f,  0.404602f,  0.172973f, -0.090511f,  0.404602f,  0.279473f,
 -0.124823f,  0.475553f,  0.090932f, -0.154433f,  0.475553f,  0.000246f,
 -0.237440f,  0.404602f,  0.172973f, -0.154433f,  0.475553f,  0.000246f,
 -0.293763f,  0.404602f,  0.000468f, -0.237440f,  0.404602f,  0.172973f,
 -0.154433f,  0.475553f,  0.000246f, -0.125112f,  0.475553f, -0.090534f,
 -0.293763f,  0.404602f,  0.000468f, -0.125112f,  0.475553f, -0.090534f,
 -0.237989f,  0.404602f, -0.172216f, -0.293763f,  0.404602f,  0.000468f,
 -0.125112f,  0.475553f, -0.090534f, -0.048050f,  0.475553f, -0.146768f,
 -0.237989f,  0.404602f, -0.172216f, -0.048050f,  0.475553f, -0.146768f,
 -0.091401f,  0.404602f, -0.279183f, -0.237989f,  0.404602f, -0.172216f,
 -0.048050f,  0.475553f, -0.146768f,  0.047348f,  0.475553f, -0.146995f,
 -0.091401f,  0.404602f, -0.279183f,  0.047348f,  0.475553f, -0.146995f,
  0.090066f,  0.404602f, -0.279616f, -0.091401f,  0.404602f, -0.279183f,
  0.047348f,  0.475553f, -0.146995f,  0.124678f,  0.475553f, -0.091131f,
  0.090066f,  0.404602f, -0.279616f,  0.124678f,  0.475553f, -0.091131f,
  0.237164f,  0.404602f, -0.173351f,  0.090066f,  0.404602f, -0.279616f,
  0.124678f,  0.475553f, -0.091131f,  0.154432f,  0.475553f, -0.000492f,
  0.237164f,  0.404602f, -0.173351f,  0.154432f,  0.475553f, -0.000492f,
  0.293762f,  0.404602f, -0.000936f,  0.237164f,  0.404602f, -0.173351f,
  0.293764f,  0.404602f,  0.000000f,  0.237715f,  0.404602f,  0.172594f,
  0.404368f,  0.294086f,  0.000000f,  0.237715f,  0.404602f,  0.172594f,
  0.327216f,  0.294086f,  0.237577f,  0.404368f,  0.294086f,  0.000000f,
  0.237715f,  0.404602f,  0.172594f,  0.090956f,  0.404602f,  0.279328f,
  0.327216f,  0.294086f,  0.237577f,  0.090956f,  0.404602f,  0.279328f,
  0.125202f,  0.294086f,  0.384497f,  0.327216f,  0.294086f,  0.237577f,
  0.090956f,  0.404602f,  0.279328f, -0.090511f,  0.404602f,  0.279473f,
  0.125202f,  0.294086f,  0.384497f, -0.090511f,  0.404602f,  0.279473f,
 -0.124589f,  0.294086f,  0.384696f,  0.125202f,  0.294086f,  0.384497f,
 -0.090511f,  0.404602f,  0.279473f, -0.237440f,  0.404602f,  0.172973f,
 -0.124589f,  0.294086f,  0.384696f, -0.237440f,  0.404602f,  0.172973f,
 -0.326838f,  0.294086f,  0.238098f, -0.124589f,  0.294086f,  0.384696f,
 -0.237440f,  0.404602f,  0.172973f, -0.293763f,  0.404602f,  0.000468f,
 -0.326838f,  0.294086f,  0.238098f, -0.293763f,  0.404602f,  0.000468f,
 -0.404368f,  0.294086f,  0.000644f, -0.326838f,  0.294086f,  0.238098f,
 -0.293763f,  0.404602f,  0.000468f, -0.237989f,  0.404602f, -0.172216f,
 -0.404368f,  0.294086f,  0.000644f, -0.237989f,  0.404602f, -0.172216f,
 -0.327594f,  0.294086f, -0.237056f, -0.404368f,  0.294086f,  0.000644f,
 -0.237989f,  0.404602f, -0.172216f, -0.091401f,  0.404602f, -0.279183f,
 -0.327594f,  0.294086f, -0.237056f, -0.091401f,  0.404602f, -0.279183f,
 -0.125814f,  0.294086f, -0.384297f, -0.327594f,  0.294086f, -0.237056f,
 -0.091401f,  0.404602f, -0.279183f,  0.090066f,  0.404602f, -0.279616f,
 -0.125814f,  0.294086f, -0.384297f,  0.090066f,  0.404602f, -0.279616f,
  0.123976f,  0.294086f, -0.384894f, -0.125814f,  0.294086f, -0.384297f,
  0.090066f,  0.404602f, -0.279616f,  0.237164f,  0.404602f, -0.173351f,
  0.123976f,  0.294086f, -0.384894f,  0.237164f,  0.404602f, -0.173351f,
  0.326458f,  0.294086f, -0.238618f,  0.123976f,  0.294086f, -0.384894f,
  0.237164f,  0.404602f, -0.173351f,  0.293762f,  0.404602f, -0.000936f,
  0.326458f,  0.294086f, -0.238618f,  0.293762f,  0.404602f, -0.000936f,
  0.404366f,  0.294086f, -0.001288f,  0.326458f,  0.294086f, -0.238618f,
  0.404368f,  0.294086f,  0.000000f,  0.327216f,  0.294086f,  0.237577f,
  0.475430f,  0.154811f,  0.000000f,  0.327216f,  0.294086f,  0.237577f,
  0.384720f,  0.154811f,  0.279328f,  0.475430f,  0.154811f,  0.000000f,
  0.327216f,  0.294086f,  0.237577f,  0.125202f,  0.294086f,  0.384497f,
  0.384720f,  0.154811f,  0.279328f,  0.125202f,  0.294086f,  0.384497f,
  0.147204f,  0.154811f,  0.452067f,  0.384720f,  0.154811f,  0.279328f,
  0.125202f,  0.294086f,  0.384497f, -0.124589f,  0.294086f,  0.384696f,
  0.147204f,  0.154811f,  0.452067f, -0.124589f,  0.294086f,  0.384696f,
 -0.146484f,  0.154811f,  0.452301f,  0.147204f,  0.154811f,  0.452067f,
 -0.124589f,  0.294086f,  0.384696f, -0.326838f,  0.294086f,  0.238098f,
 -0.146484f,  0.154811f,  0.452301f, -0.326838f,  0.294086f,  0.238098f,
 -0.384274f,  0.154811f,  0.279940f, -0.146484f,  0.154811f,  0.452301f,
 -0.326838f,  0.294086f,  0.238098f, -0.404368f,  0.294086f,  0.000644f,
 -0.384274f,  0.154811f,  0.279940f, -0.404368f,  0.294086f,  0.000644f,
 -0.475429f,  0.154811f,  0.000757f, -0.384274f,  0.154811f,  0.279940f,
 -0.404368f,  0.294086f,  0.000644f, -0.327594f,  0.294086f, -0.237056f,
 -0.475429f,  0.154811f,  0.000757f, -0.327594f,  0.294086f, -0.237056f,
 -0.385164f,  0.154811f, -0.278715f, -0.475429f,  0.154811f,  0.000757f,
 -0.327594f,  0.294086f, -0.237056f, -0.125814f,  0.294086f, -0.384297f,
 -0.385164f,  0.154811f, -0.278715f, -0.125814f,  0.294086f, -0.384297f,
 -0.147924f,  0.154811f, -0.451832f, -0.385164f,  0.154811f, -0.278715f,
 -0.125814f,  0.294086f, -0.384297f,  0.123976f,  0.294086f, -0.384894f,
 -0.147924f,  0.154811f, -0.451832f,  0.123976f,  0.294086f, -0.384894f,
  0.145763f,  0.154811f, -0.452533f, -0.147924f,  0.154811f, -0.451832f,
  0.123976f,  0.294086f, -0.384894f,  0.326458f,  0.294086f, -0.238618f,
  0.145763f,  0.154811f, -0.452533f,  0.326458f,  0.294086f, -0.238618f,
  0.383828f,  0.154811f, -0.280552f,  0.145763f,  0.154811f, -0.452533f,
  0.326458f,  0.294086f, -0.238618f,  0.404366f,  0.294086f, -0.001288f,
  0.383828f,  0.154811f, -0.280552f,  0.404366f,  0.294086f, -0.001288f,
  0.475427f,  0.154811f, -0.001514f,  0.383828f,  0.154811f, -0.280552f,
  0.475430f,  0.154811f,  0.000000f,  0.384720f,  0.154811f,  0.279328f,
  0.500000f,  0.000398f,  0.000000f,  0.384720f,  0.154811f,  0.279328f,
  0.404602f,  0.000398f,  0.293764f,  0.500000f,  0.000398f,  0.000000f,
  0.384720f,  0.154811f,  0.279328f,  0.147204f,  0.154811f,  0.452067f,
  0.404602f,  0.000398f,  0.293764f,  0.147204f,  0.154811f,  0.452067f,
  0.154811f,  0.000398f,  0.475430f,  0.404602f,  0.000398f,  0.293764f,
  0.147204f,  0.154811f,  0.452067f, -0.146484f,  0.154811f,  0.452301f,
  0.154811f,  0.000398f,  0.475430f, -0.146484f,  0.154811f,  0.452301f,
 -0.154054f,  0.000398f,  0.475676f,  0.154811f,  0.000398f,  0.475430f,
 -0.146484f,  0.154811f,  0.452301f, -0.384274f,  0.154811f,  0.279940f,
 -0.154054f,  0.000398f,  0.475676f, -0.384274f,  0.154811f,  0.279940f,
 -0.404134f,  0.000398f,  0.294408f, -0.154054f,  0.000398f,  0.475676f,
 -0.384274f,  0.154811f,  0.279940f, -0.475429f,  0.154811f,  0.000757f,
 -0.404134f,  0.000398f,  0.294408f, -0.475429f,  0.154811f,  0.000757f,
 -0.499999f,  0.000398f,  0.000796f, -0.404134f,  0.000398f,  0.294408f,
 -0.475429f,  0.154811f,  0.000757f, -0.385164f,  0.154811f, -0.278715f,
 -0.499999f,  0.000398f,  0.000796f, -0.385164f,  0.154811f, -0.278715f,
 -0.405069f,  0.000398f, -0.293119f, -0.499999f,  0.000398f,  0.000796f,
 -0.385164f,  0.154811f, -0.278715f, -0.147924f,  0.154811f, -0.451832f,
 -0.405069f,  0.000398f, -0.293119f, -0.147924f,  0.154811f, -0.451832f,
 -0.155568f,  0.000398f, -0.475182f, -0.405069f,  0.000398f, -0.293119f,
 -0.147924f,  0.154811f, -0.451832f,  0.145763f,  0.154811f, -0.452533f,
 -0.155568f,  0.000398f, -0.475182f,  0.145763f,  0.154811f, -0.452533f,
  0.153296f,  0.000398f, -0.475920f, -0.155568f,  0.000398f, -0.475182f,
  0.145763f,  0.154811f, -0.452533f,  0.383828f,  0.154811f, -0.280552f,
  0.153296f,  0.000398f, -0.475920f,  0.383828f,  0.154811f, -0.280552f,
  0.403664f,  0.000398f, -0.295051f,  0.153296f,  0.000398f, -0.475920f,
  0.383828f,  0.154811f, -0.280552f,  0.475427f,  0.154811f, -0.001514f,
  0.403664f,  0.000398f, -0.295051f,  0.475427f,  0.154811f, -0.001514f,
  0.499997f,  0.000398f, -0.001592f,  0.403664f,  0.000398f, -0.295051f,
  0.500000f,  0.000398f,  0.000000f,  0.404602f,  0.000398f,  0.293764f,
  0.475676f, -0.154054f,  0.000000f,  0.404602f,  0.000398f,  0.293764f,
  0.384919f, -0.154054f,  0.279473f,  0.475676f, -0.154054f,  0.000000f,
  0.404602f,  0.000398f,  0.293764f,  0.154811f,  0.000398f,  0.475430f,
  0.384919f, -0.154054f,  0.279473f,  0.154811f,  0.000398f,  0.475430f,
  0.147280f, -0.154054f,  0.452301f,  0.384919f, -0.154054f,  0.279473f,
  0.154811f,  0.000398f,  0.475430f, -0.154054f,  0.000398f,  0.475676f,
  0.147280f, -0.154054f,  0.452301f, -0.154054f,  0.000398f,  0.475676f,
 -0.146560f, -0.154054f,  0.452535f,  0.147280f, -0.154054f,  0.452301f,
 -0.154054f,  0.000398f,  0.475676f, -0.404134f,  0.000398f,  0.294408f,
 -0.146560f, -0.154054f,  0.452535f, -0.404134f,  0.000398f,  0.294408f,
 -0.384473f, -0.154054f,  0.280085f, -0.146560f, -0.154054f,  0.452535f,
 -0.404134f,  0.000398f,  0.294408f, -0.499999f,  0.000398f,  0.000796f,
 -0.384473f, -0.154054f,  0.280085f, -0.499999f,  0.000398f,  0.000796f,
 -0.475675f, -0.154054f,  0.000758f, -0.384473f, -0.154054f,  0.280085f,
 -0.499999f,  0.000398f,  0.000796f, -0.405069f,  0.000398f, -0.293119f,
 -0.475675f, -0.154054f,  0.000758f, -0.405069f,  0.000398f, -0.293119f,
 -0.385363f, -0.154054f, -0.278859f, -0.475675f, -0.154054f,  0.000758f,
 -0.405069f,  0.000398f, -0.293119f, -0.155568f,  0.000398f, -0.475182f,
 -0.385363f, -0.154054f, -0.278859f, -0.155568f,  0.000398f, -0.475182f,
 -0.148000f, -0.154054f, -0.452066f, -0.385363f, -0.154054f, -0.278859f,
 -0.155568f,  0.000398f, -0.475182f,  0.153296f,  0.000398f, -0.475920f,
 -0.148000f, -0.154054f, -0.452066f,  0.153296f,  0.000398f, -0.475920f,
  0.145839f, -0.154054f, -0.452768f, -0.148000f, -0.154054f, -0.452066f,
  0.153296f,  0.000398f, -0.475920f,  0.403664f,  0.000398f, -0.295051f,
  0.145839f, -0.154054f, -0.452768f,  0.403664f,  0.000398f, -0.295051f,
  0.384027f, -0.154054f, -0.280697f,  0.145839f, -0.154054f, -0.452768f,
  0.403664f,  0.000398f, -0.295051f,  0.499997f,  0.000398f, -0.001592f,
  0.384027f, -0.154054f, -0.280697f,  0.499997f,  0.000398f, -0.001592f,
  0.475673f, -0.154054f, -0.001515f,  0.384027f, -0.154054f, -0.280697f,
  0.475676f, -0.154054f,  0.000000f,  0.384919f, -0.154054f,  0.279473f,
  0.404836f, -0.293441f,  0.000000f,  0.384919f, -0.154054f,  0.279473f,
  0.327595f, -0.293441f,  0.237852f,  0.404836f, -0.293441f,  0.000000f,
  0.384919f, -0.154054f,  0.279473f,  0.147280f, -0.154054f,  0.452301f,
  0.327595f, -0.293441f,  0.237852f,  0.147280f, -0.154054f,  0.452301f,
  0.125346f, -0.293441f,  0.384942f,  0.327595f, -0.293441f,  0.237852f,
  0.147280f, -0.154054f,  0.452301f, -0.146560f, -0.154054f,  0.452535f,
  0.125346f, -0.293441f,  0.384942f, -0.146560f, -0.154054f,  0.452535f,
 -0.124733f, -0.293441f,  0.385141f,  0.125346f, -0.293441f,  0.384942f,
 -0.146560f, -0.154054f,  0.452535f, -0.384473f, -0.154054f,  0.280085f,
 -0.124733f, -0.293441f,  0.385141f, -0.384473f, -0.154054f,  0.280085f,
 -0.327216f, -0.293441f,  0.238374f, -0.124733f, -0.293441f,  0.385141f,
 -0.384473f, -0.154054f,  0.280085f, -0.475675f, -0.154054f,  0.000758f,
 -0.327216f, -0.293441f,  0.238374f, -0.475675f, -0.154054f,  0.000758f,
 -0.404835f, -0.293441f,  0.000645f, -0.327216f, -0.293441f,  0.238374f,
 -0.475675f, -0.154054f,  0.000758f, -0.385363f, -0.154054f, -0.278859f,
 -0.404835f, -0.293441f,  0.000645f, -0.385363f, -0.154054f, -0.278859f,
 -0.327973f, -0.293441f, -0.237330f, -0.404835f, -0.293441f,  0.000645f,
 -0.385363f, -0.154054f, -0.278859f, -0.148000f, -0.154054f, -0.452066f,
 -0.327973f, -0.293441f, -0.237330f, -0.148000f, -0.154054f, -0.452066f,
 -0.125959f, -0.293441f, -0.384742f, -0.327973f, -0.293441f, -0.237330f,
 -0.148000f, -0.154054f, -0.452066f,  0.145839f, -0.154054f, -0.452768f,
 -0.125959f, -0.293441f, -0.384742f,  0.145839f, -0.154054f, -0.452768f,
  0.124120f, -0.293441f, -0.385339f, -0.125959f, -0.293441f, -0.384742f,
  0.145839f, -0.154054f, -0.452768f,  0.384027f, -0.154054f, -0.280697f,
  0.124120f, -0.293441f, -0.385339f,  0.384027f, -0.154054f, -0.280697f,
  0.326836f, -0.293441f, -0.238894f,  0.124120f, -0.293441f, -0.385339f,
  0.384027f, -0.154054f, -0.280697f,  0.475673f, -0.154054f, -0.001515f,
  0.326836f, -0.293441f, -0.238894f,  0.475673f, -0.154054f, -0.001515f,
  0.404834f, -0.293441f, -0.001289f,  0.326836f, -0.293441f, -0.238894f,
  0.404836f, -0.293441f,  0.000000f,  0.327595f, -0.293441f,  0.237852f,
  0.294408f, -0.404134f,  0.000000f,  0.327595f, -0.293441f,  0.237852f,
  0.238236f, -0.404134f,  0.172973f,  0.294408f, -0.404134f,  0.000000f,
  0.327595f, -0.293441f,  0.237852f,  0.125346f, -0.293441f,  0.384942f,
  0.238236f, -0.404134f,  0.172973f,  0.125346f, -0.293441f,  0.384942f,
  0.091155f, -0.404134f,  0.279940f,  0.238236f, -0.404134f,  0.172973f,
  0.125346f, -0.293441f,  0.384942f, -0.124733f, -0.293441f,  0.385141f,
  0.091155f, -0.404134f,  0.279940f, -0.124733f, -0.293441f,  0.385141f,
 -0.090709f, -0.404134f,  0.280085f,  0.091155f, -0.404134f,  0.279940f,
 -0.124733f, -0.293441f,  0.385141f, -0.327216f, -0.293441f,  0.238374f,
 -0.090709f, -0.404134f,  0.280085f, -0.327216f, -0.293441f,  0.238374f,
 -0.237960f, -0.404134f,  0.173352f, -0.090709f, -0.404134f,  0.280085f,
 -0.327216f, -0.293441f,  0.238374f, -0.404835f, -0.293441f,  0.000645f,
 -0.237960f, -0.404134f,  0.173352f, -0.404835f, -0.293441f,  0.000645f,
 -0.294407f, -0.404134f,  0.000469f, -0.237960f, -0.404134f,  0.173352f,
 -0.404835f, -0.293441f,  0.000645f, -0.327973f, -0.293441f, -0.237330f,
 -0.294407f, -0.404134f,  0.000469f, -0.327973f, -0.293441f, -0.237330f,
 -0.238511f, -0.404134f, -0.172593f, -0.294407f, -0.404134f,  0.000469f,
 -0.327973f, -0.293441f, -0.237330f, -0.125959f, -0.293441f, -0.384742f,
 -0.238511f, -0.404134f, -0.172593f, -0.125959f, -0.293441f, -0.384742f,
 -0.091601f, -0.404134f, -0.279795f, -0.238511f, -0.404134f, -0.172593f,
 -0.125959f, -0.293441f, -0.384742f,  0.124120f, -0.293441f, -0.385339f,
 -0.091601f, -0.404134f, -0.279795f,  0.124120f, -0.293441f, -0.385339f,
  0.090263f, -0.404134f, -0.280229f, -0.091601f, -0.404134f, -0.279795f,
  0.124120f, -0.293441f, -0.385339f,  0.326836f, -0.293441f, -0.238894f,
  0.090263f, -0.404134f, -0.280229f,  0.326836f, -0.293441f, -0.238894f,
  0.237684f, -0.404134f, -0.173731f,  0.090263f, -0.404134f, -0.280229f,
  0.326836f, -0.293441f, -0.238894f,  0.404834f, -0.293441f, -0.001289f,
  0.237684f, -0.404134f, -0.173731f,  0.404834f, -0.293441f, -0.001289f,
  0.294406f, -0.404134f, -0.000938f,  0.237684f, -0.404134f, -0.173731f,
  0.294408f, -0.404134f,  0.000000f,  0.238236f, -0.404134f,  0.172973f,
  0.155190f, -0.475306f,  0.000000f,  0.238236f, -0.404134f,  0.172973f,
  0.125580f, -0.475306f,  0.091178f,  0.155190f, -0.475306f,  0.000000f,
  0.238236f, -0.404134f,  0.172973f,  0.091155f, -0.404134f,  0.279940f,
  0.125580f, -0.475306f,  0.091178f,  0.091155f, -0.404134f,  0.279940f,
  0.048050f, -0.475306f,  0.147564f,  0.125580f, -0.475306f,  0.091178f,
  0.091155f, -0.404134f,  0.279940f, -0.090709f, -0.404134f,  0.280085f,
  0.048050f, -0.475306f,  0.147564f, -0.090709f, -0.404134f,  0.280085f,
 -0.047815f, -0.475306f,  0.147640f,  0.048050f, -0.475306f,  0.147564f,
 -0.090709f, -0.404134f,  0.280085f, -0.237960f, -0.404134f,  0.173352f,
 -0.047815f, -0.475306f,  0.147640f, -0.237960f, -0.404134f,  0.173352f,
 -0.125435f, -0.475306f,  0.091378f, -0.047815f, -0.475306f,  0.147640f,
 -0.237960f, -0.404134f,  0.173352f, -0.294407f, -0.404134f,  0.000469f,
 -0.125435f, -0.475306f,  0.091378f, -0.294407f, -0.404134f,  0.000469f,
 -0.155190f, -0.475306f,  0.000247f, -0.125435f, -0.475306f,  0.091378f,
 -0.294407f, -0.404134f,  0.000469f, -0.238511f, -0.404134f, -0.172593f,
 -0.155190f, -0.475306f,  0.000247f, -0.238511f, -0.404134f, -0.172593f,
 -0.125725f, -0.475306f, -0.090978f, -0.155190f, -0.475306f,  0.000247f,
 -0.238511f, -0.404134f, -0.172593f, -0.091601f, -0.404134f, -0.279795f,
 -0.125725f, -0.475306f, -0.090978f, -0.091601f, -0.404134f, -0.279795f,
 -0.048285f, -0.475306f, -0.147487f, -0.125725f, -0.475306f, -0.090978f,
 -0.091601f, -0.404134f, -0.279795f,  0.090263f, -0.404134f, -0.280229f,
 -0.048285f, -0.475306f, -0.147487f,  0.090263f, -0.404134f, -0.280229f,
  0.047580f, -0.475306f, -0.147716f, -0.048285f, -0.475306f, -0.147487f,
  0.090263f, -0.404134f, -0.280229f,  0.237684f, -0.404134f, -0.173731f,
  0.047580f, -0.475306f, -0.147716f,  0.237684f, -0.404134f, -0.173731f,
  0.125289f, -0.475306f, -0.091578f,  0.047580f, -0.475306f, -0.147716f,
  0.237684f, -0.404134f, -0.173731f,  0.294406f, -0.404134f, -0.000938f,
  0.125289f, -0.475306f, -0.091578f,  0.294406f, -0.404134f, -0.000938f,
  0.155189f, -0.475306f, -0.000494f,  0.125289f, -0.475306f, -0.091578f,
  0.155190f, -0.475306f,  0.000000f,  0.125580f, -0.475306f,  0.091178f,
  0.000796f, -0.499999f,  0.000000f,  0.125580f, -0.475306f,  0.091178f,
  0.000644f, -0.499999f,  0.000468f,  0.000796f, -0.499999f,  0.000000f,
  0.125580f, -0.475306f,  0.091178f,  0.048050f, -0.475306f,  0.147564f,
  0.000644f, -0.499999f,  0.000468f,  0.048050f, -0.475306f,  0.147564f,
  0.000247f, -0.499999f,  0.000757f,  0.000644f, -0.499999f,  0.000468f,
  0.048050f, -0.475306f,  0.147564f, -0.047815f, -0.475306f,  0.147640f,
  0.000247f, -0.499999f,  0.000757f, -0.047815f, -0.475306f,  0.147640f,
 -0.000245f, -0.499999f,  0.000757f,  0.000247f, -0.499999f,  0.000757f,
 -0.047815f, -0.475306f,  0.147640f, -0.125435f, -0.475306f,  0.091378f,
 -0.000245f, -0.499999f,  0.000757f, -0.125435f, -0.475306f,  0.091378f,
 -0.000644f, -0.499999f,  0.000469f, -0.000245f, -0.499999f,  0.000757f,
 -0.125435f, -0.475306f,  0.091378f, -0.155190f, -0.475306f,  0.000247f,
 -0.000644f, -0.499999f,  0.000469f, -0.155190f, -0.475306f,  0.000247f,
 -0.000796f, -0.499999f,  0.000001f, -0.000644f, -0.499999f,  0.000469f,
 -0.155190f, -0.475306f,  0.000247f, -0.125725f, -0.475306f, -0.090978f,
 -0.000796f, -0.499999f,  0.000001f, -0.125725f, -0.475306f, -0.090978f,
 -0.000645f, -0.499999f, -0.000467f, -0.000796f, -0.499999f,  0.000001f,
 -0.125725f, -0.475306f, -0.090978f, -0.048285f, -0.475306f, -0.147487f,
 -0.000645f, -0.499999f, -0.000467f, -0.048285f, -0.475306f, -0.147487f,
 -0.000248f, -0.499999f, -0.000757f, -0.000645f, -0.499999f, -0.000467f,
 -0.048285f, -0.475306f, -0.147487f,  0.047580f, -0.475306f, -0.147716f,
 -0.000248f, -0.499999f, -0.000757f,  0.047580f, -0.475306f, -0.147716f,
  0.000244f, -0.499999f, -0.000758f, -0.000248f, -0.499999f, -0.000757f,
  0.047580f, -0.475306f, -0.147716f,  0.125289f, -0.475306f, -0.091578f,
  0.000244f, -0.499999f, -0.000758f,  0.125289f, -0.475306f, -0.091578f,
  0.000643f, -0.499999f, -0.000470f,  0.000244f, -0.499999f, -0.000758f,
  0.125289f, -0.475306f, -0.091578f,  0.155189f, -0.475306f, -0.000494f,
  0.000643f, -0.499999f, -0.000470f,  0.155189f, -0.475306f, -0.000494f,
  0.000796f, -0.499999f, -0.000003f,  0.000643f, -0.499999f, -0.000470f,
  0.000796f, -0.500000f,  0.000000f,  0.000644f, -0.500000f,  0.000468f,
  0.000000f, -0.500000f,  0.000000f,  0.000644f, -0.500000f,  0.000468f,
  0.000247f, -0.500000f,  0.000757f,  0.000000f, -0.500000f,  0.000000f,
  0.000247f, -0.500000f,  0.000757f, -0.000245f, -0.500000f,  0.000757f,
  0.000000f, -0.500000f,  0.000000f, -0.000245f, -0.500000f,  0.000757f,
 -0.000644f, -0.500000f,  0.000469f,  0.000000f, -0.500000f,  0.000000f,
 -0.000644f, -0.500000f,  0.000469f, -0.000796f, -0.500000f,  0.000001f,
  0.000000f, -0.500000f,  0.000000f, -0.000796f, -0.500000f,  0.000001f,
 -0.000645f, -0.500000f, -0.000467f,  0.000000f, -0.500000f,  0.000000f,
 -0.000645f, -0.500000f, -0.000467f, -0.000248f, -0.500000f, -0.000757f,
  0.000000f, -0.500000f,  0.000000f, -0.000248f, -0.500000f, -0.000757f,
  0.000244f, -0.500000f, -0.000758f,  0.000000f, -0.500000f,  0.000000f,
  0.000244f, -0.500000f, -0.000758f,  0.000643f, -0.500000f, -0.000470f,
  0.000000f, -0.500000f,  0.000000f,  0.000643f, -0.500000f, -0.000470f,
  0.000796f, -0.500000f, -0.000003f,  0.000000f, -0.500000f,  0.000000f
};

int sphereVerticesLength = sizeof(sphereVertices) / sizeof(float);

//
// Each group of three values specifies a sphere triangle
//
int sphereElements[] = {
   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,
  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,
  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,
  90,  91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104,
 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224,
 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254,
 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269,
 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284,
 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299,
 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314,
 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329,
 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344,
 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359,
 360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374,
 375, 376, 377, 378, 379, 380, 381, 382, 383, 384, 385, 386, 387, 388, 389,
 390, 391, 392, 393, 394, 395, 396, 397, 398, 399, 400, 401, 402, 403, 404,
 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419,
 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434,
 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449,
 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 464,
 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479,
 480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494,
 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509,
 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524,
 525, 526, 527, 528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539,
 540, 541, 542, 543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554,
 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, 565, 566, 567, 568, 569,
 570, 571, 572, 573, 574, 575, 576, 577, 578, 579, 580, 581, 582, 583, 584,
 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597, 598, 599
};

int sphereElementsLength = sizeof(sphereElements) / sizeof(int);

//
// makeSphere() - create a sphere object
//
void makeSphere( Canvas &C )
{
    int i;

    for( i = 0; i < sphereElementsLength - 2; i += 3 ) {

        // Calculate the base indices of the three vertices
        int point1 = 3 * sphereElements[i];     // slots 0, 1, 2
        int point2 = 3 * sphereElements[i + 1]; // slots 3, 4, 5
        int point3 = 3 * sphereElements[i + 2]; // slots 6, 7, 8

        Vertex p1 = { sphereVertices[point1 + 0],
                      sphereVertices[point1 + 1],
                      sphereVertices[point1 + 2] };

        Vertex p2 = { sphereVertices[point2 + 0],
                      sphereVertices[point2 + 1],
                      sphereVertices[point2 + 2] };

        Vertex p3 = { sphereVertices[point3 + 0],
                      sphereVertices[point3 + 1],
                      sphereVertices[point3 + 2] };

        // Calculate the base indices of the three normals
        int normal1 = 3 * sphereElements[i];     // slots 0, 1, 2
        int normal2 = 3 * sphereElements[i + 1]; // slots 3, 4, 5
        int normal3 = 3 * sphereElements[i + 2]; // slots 6, 7, 8

        Normal n1 = { sphereVertices[normal1 + 0],
                      sphereVertices[normal1 + 1],
                      sphereVertices[normal1 + 2] };

        Normal n2 = { sphereVertices[normal2 + 0],
                      sphereVertices[normal2 + 1],
                      sphereVertices[normal2 + 2] };

        Normal n3 = { sphereVertices[normal3 + 0],
                      sphereVertices[normal3 + 1],
                      sphereVertices[normal3 + 2] };

        // Add triangle and vertex normals
        C.addTriangleWithNorms( p1, n1, p2, n2, p3, n3 );
    }
}

///
// drawSphere
//
// Invoked whenever the sphere must be redrawn
//
// @param pshader - shader program for Phong shading
// @param tshader - shader program for texture mapping
// @param bset    - the BufferSet containing the object's data
///
void drawSphere( GLuint pshader, GLuint tshader, BufferSet &bset ) {

    // extern declarations for things defined in the main
    // program source file that we need to use here
    extern void selectBuffers( GLuint, BufferSet & );
    extern GLfloat angles;
    extern GLfloat xlate[3];
    extern GLfloat camX;
	extern GLfloat camZ;

    // by default, the sphere is drawn using Phong shading.
    // IF YOU ARE DOING THE EXTRA-CREDIT OPTION, this code must
    // be modified to use the texture-mapping shader and to call
    // setUpTextures() instead of setUpPhong().  See the code for
    // OBJ_QUAD (in the Shapes module) for the changes.

    glUseProgram( pshader );

    // set up viewing and projection parameters
    setUpProjection( pshader );

    // set up the Phong shading information
    setUpPhong( pshader, OBJ_SPHERE );

    // set up the camera
    setUpCamera( pshader,
        (Tuple) { camX, 3.0f, camZ },
        (Tuple) { 0.0f, 1.0f, 0.0f },
        (Tuple) { 0.0f, 1.0f, 0.0f }
    );

    setUpTransforms( pshader,
        (Tuple) { 1.5f, 1.5f, 1.5f },
        (Tuple) { angles, angles, angles },
        (Tuple) { xlate[0], xlate[1], xlate[2] }
    );

    // draw it
    selectBuffers( pshader, bset );
    glDrawElements( GL_TRIANGLES, bset.numElements,
        GL_UNSIGNED_INT, (void *)0 );

}

///
//  Shapes.cpp
//
//  Routines for tessellating a number of basic shapes
//
//  Students are to supply their implementations for the functions in
//  this file using the function addTriangle() to do the tessellation.
//
//  Author:  Warren R. Carithers 2017/04/01.
//
//  Contributor:  Dmytro Rudenkyy
///

#include <cmath>
using namespace std;
#include <iostream>

// Canvas.h pulls in all the OpenGL/GLFW/etc. header files for us
#include "Shapes.h"
#include "Canvas.h"
#include "Vertex.h"

#define PI 3.14159265358979323846

///
// makeCube - Create a unit cube, centered at the origin, with a given number
// of subdivisions in each direction on each face.
//
// @param subdivision - number of equal subdivisions to be made in each 
//        direction along each face
//
// Can only use calls to C.addTriangle()
///
void makeCube( Canvas &C, int subdivisions )
{
    if( subdivisions < 1 )
        subdivisions = 1;

    float Q[subdivisions+1];
    Vertex P[subdivisions+1][subdivisions+1];
    float u=0,v=0;
    //front face: z=0.5
    for(int i=0;i<=subdivisions;i++){//create vertices
		//determine y values
		u=((float)i)/subdivisions;
		Q[i]=((1-u)*-.5)+u*.5;
		for(int j=0;j<=subdivisions;j++){
			//determine x values
			v=((float)j)/subdivisions;
			P[i][j].x=((1-v)*-.5)+v*.5;
			P[i][j].y=Q[i];
			P[i][j].z=0.5;
		}
	}
	for(int i=0;i<subdivisions;i++){//add triangles
		for(int j=0;j<subdivisions;j++){
			C.addTriangle(P[i][j], P[i+1][j+1], P[i+1][j]);//upper triangle
			C.addTriangle(P[i][j], P[i][j+1], P[i+1][j+1]);//lower triangle
		}
	}
	
	//back face: z=-0.5
    for(int i=0;i<=subdivisions;i++){//create vertices
		//determine y values
		u=((float)i)/subdivisions;
		Q[i]=((1-u)*-.5)+u*.5;
		for(int j=0;j<=subdivisions;j++){
			//determine x values
			v=((float)j)/subdivisions;
			P[i][j].x=((1-v)*-.5)+v*.5;
			P[i][j].y=Q[i];
			P[i][j].z=-0.5;
		}
	}
	for(int i=0;i<subdivisions;i++){//add triangles
		for(int j=0;j<subdivisions;j++){
			C.addTriangle(P[i][j], P[i+1][j], P[i+1][j+1]);//upper triangle
			C.addTriangle(P[i][j], P[i+1][j+1], P[i][j+1]);//lower triangle
		}
	}
	
	//left face: x=-0.5
    for(int i=0;i<=subdivisions;i++){//create vertices
		//determine y values
		u=((float)i)/subdivisions;
		Q[i]=((1-u)*-.5)+u*.5;
		for(int j=0;j<=subdivisions;j++){
			//determine z values
			v=((float)j)/subdivisions;
			P[i][j].z=((1-v)*-.5)+v*.5;
			P[i][j].y=Q[i];
			P[i][j].x=-0.5;
		}
	}
	for(int i=0;i<subdivisions;i++){//add triangles
		for(int j=0;j<subdivisions;j++){
			C.addTriangle(P[i][j], P[i+1][j+1], P[i+1][j]);//upper triangle
			C.addTriangle(P[i][j], P[i][j+1], P[i+1][j+1]);//lower triangle
		}
	}
	
	//right face: x = 0.5
    for(int i=0;i<=subdivisions;i++){//create vertices
		//determine y values
		u=((float)i)/subdivisions;
		Q[i]=((1-u)*-.5)+u*.5;
		for(int j=0;j<=subdivisions;j++){
			//determine z values
			v=((float)j)/subdivisions;
			P[i][j].z=((1-v)*-.5)+v*.5;
			P[i][j].y=Q[i];
			P[i][j].x=0.5;
		}
	}
	for(int i=0;i<subdivisions;i++){//add triangles
		for(int j=0;j<subdivisions;j++){
			C.addTriangle(P[i][j], P[i+1][j], P[i+1][j+1]);//upper triangle
			C.addTriangle(P[i][j], P[i+1][j+1], P[i][j+1]);//lower triangle
		}
	}
	
	
	//top face: y=0.5
    for(int i=0;i<=subdivisions;i++){//create vertices
		//determine x values
		u=((float)i)/subdivisions;
		Q[i]=((1-u)*-.5)+u*.5;
		for(int j=0;j<=subdivisions;j++){
			//determine z values
			v=((float)j)/subdivisions;
			P[i][j].z=((1-v)*-.5)+v*.5;
			P[i][j].x=Q[i];
			P[i][j].y=0.5;
		}
	}
	for(int i=0;i<subdivisions;i++){//add triangles
		for(int j=0;j<subdivisions;j++){
			C.addTriangle(P[i][j], P[i+1][j+1], P[i+1][j]);//upper triangle
			C.addTriangle(P[i][j], P[i][j+1], P[i+1][j+1]);//lower triangle
		}
	}
	
	//bottom face: y=-0.5
    for(int i=0;i<=subdivisions;i++){//create vertices
		//determine x values
		u=((float)i)/subdivisions;
		Q[i]=((1-u)*-.5)+u*.5;
		for(int j=0;j<=subdivisions;j++){
			//determine z values
			v=((float)j)/subdivisions;
			P[i][j].z=((1-v)*-.5)+v*.5;
			P[i][j].x=Q[i];
			P[i][j].y=-0.5;
		}
	}
	for(int i=0;i<subdivisions;i++){//add triangles
		for(int j=0;j<subdivisions;j++){
			C.addTriangle(P[i][j], P[i+1][j], P[i+1][j+1]);//upper triangle
			C.addTriangle(P[i][j], P[i+1][j+1], P[i][j+1]);//lower triangle
		}
	}
	
}

///
// makeCylinder - Create polygons for a cylinder with unit height, centered at
// the origin, with separate number of radial subdivisions and height 
// subdivisions.
//
// @param radius - Radius of the base of the cylinder
// @param radialDivision - number of subdivisions on the radial base
// @param heightDivisions - number of subdivisions along the height
//
// Can only use calls to C.addTriangle()
///
void makeCylinder( Canvas &C, float radius, int radialDivisions, int heightDivisions )
{
    if( radialDivisions < 3 )
        radialDivisions = 3;

    if( heightDivisions < 1 )
        heightDivisions = 1;

    double alpha=0;
    
    Vertex top[radialDivisions+1], bot[radialDivisions+1];
    //centerpoints for drawing top disc
    top[0].y=0.5;
    top[0].x=0;
    top[0].z=0;
	//centerpoints for drawing bot disc
    bot[0].y=-.5;
    bot[0].x=0;
	bot[0].z=0;
	
	//calculating radius lines for top/bottom disks
	for(int i = 1; i <= radialDivisions; i ++)
	{
		float x = .5 * cos(alpha);
		float z = .5 * -sin(alpha);		//since up on the z axis is actually 
										//negative(going into the screen)
		top[i].x = x;
		top[i].z = z;
		top[i].y = .5;
		
		bot[i].x = x;
		bot[i].z = z;
		bot[i].y = -.5;
		alpha += (2 * PI)/((double)(radialDivisions));	//increment alpha to the next angle
	}
	
	for(int i = 1; i <= radialDivisions; i++)
	{
		//draw triangles representing top and bottom disks
		if(i + 1 <= radialDivisions)
		{
			C.addTriangle(top[0], top[i], top[i + 1]);
			C.addTriangle(bot[0], bot[i + 1], bot[i]);

		}
		else
		{
			C.addTriangle(top[0], top[i], top[1]);
			C.addTriangle(bot[0], bot[1], bot[i]);
		}
		
		//draw the faces
		Vertex P1, P2;
		Vertex left[heightDivisions+1];
		Vertex right[heightDivisions+1];
		//map the bottom vertices to the new P1 and P2 at every iteration
		if(i==radialDivisions){//last face
			P1=bot[i], P2=bot[1];
		}else{
			P1=bot[i], P2=bot[i+1];
		}
		left[0]=P1;		//left side of rectangular face
		right[0]=P2;	//right side of rectangular face
		float yCurr=-0.5;
		for(int j=1; j<= heightDivisions; j++){//loop divisions starting from the
											//bottom and work our way up
			yCurr+=1.0/((float)heightDivisions);
			left[j].y=yCurr;
			left[j].x=P1.x;
			left[j].z=P1.z;
			
			right[j].y=yCurr;
			right[j].x=P2.x;
			right[j].z=P2.z;
			
			
			C.addTriangle(left[j-1], right[j-1], left[j]);
			C.addTriangle(right[j-1], right[j], left[j]);
		}
	}
	
}

///
// makeCone - Create polygons for a cone with unit height, centered at the
// origin, with separate number of radial subdivisions and height 
// subdivisions.
//
// @param radius - Radius of the base of the cone
// @param radialDivision - number of subdivisions on the radial base
// @param heightDivisions - number of subdivisions along the height
//
// Can only use calls to C.addTriangle()
///
void makeCone( Canvas &C, float radius, int radialDivisions, int heightDivisions )
{
    if( radialDivisions < 3 )
        radialDivisions = 3;

    if( heightDivisions < 1 )
        heightDivisions = 1;

    double alpha=0;
    
    Vertex base[radialDivisions+1];
    
    //apex tip of the cone
    Vertex apex = {0, .5, 0};
    
	//centerpoints for drawing base disc
    base[0].y=-.5;
    base[0].x=0;
	base[0].z=0;
	
	//calculating radius lines for base disk
	for(int i = 1; i <= radialDivisions; i ++)
	{
		float x = .5 * cos(alpha);
		float z = .5 * -sin(alpha);		//since up on the z axis is actually 
										//negative(going into the screen)
		
		base[i].x = x;
		base[i].z = z;
		base[i].y = -.5;
		alpha += (2 * PI)/((double)(radialDivisions));	//increment alpha to the next angle
	}
	
	for(int i = 1; i <= radialDivisions; i++)
	{
		//draw triangles representing base disk
		if(i + 1 <= radialDivisions)
		{
			C.addTriangle(base[0], base[i+1], base[i]);

		}
		else //looping back to last triangle
		{
			C.addTriangle(base[0], base[1], base[i]);
		}
		
		//draw the faces
		Vertex P1, P2;
		Vertex left[heightDivisions+1];
		Vertex right[heightDivisions+1];
		//map the base vertices to the new P1 and P2 at every iteration
		if(i==radialDivisions){//last face
			P1=base[i], P2=base[1];
		}else{
			P1=base[i], P2=base[i+1];
		}
		left[0]=P1;		//left side of triangle face
		right[0]=P2;	//right side of triangle face
		float yCurr=-0.5;
		for(int j=1; j<= heightDivisions; j++){//loop divisions starting from the
											//bottom and work our way up
			//the original connecting triangle
			if(j == heightDivisions) {
				C.addTriangle(apex, left[j-1], right[j-1]);
				break;
			}
			//the subdivided triangles calculations
			yCurr+=1.0/((float)heightDivisions);
			alpha = yCurr - .5;

			left[j].y=yCurr;
			left[j].x=((1+alpha)*apex.x)-(alpha*left[0].x);
			left[j].z=((1+alpha)*apex.z)-(alpha*left[0].z);
			
			right[j].y=yCurr;
			right[j].x=((1+alpha)*apex.x)-(alpha*right[0].x);
			right[j].z=((1+alpha)*apex.z)-(alpha*right[0].z);
			
			C.addTriangle(left[j-1], right[j-1], left[j]);
			C.addTriangle(right[j-1], right[j], left[j]);
			
		}
	}
}

///
// makeSphere - Create sphere of a given radius, centered at the origin, 
// using spherical coordinates with separate number of thetha and 
// phi subdivisions.
//
// @param radius - Radius of the sphere
// @param slices - number of subdivisions in the theta direction
// @param stacks - Number of subdivisions in the phi direction.
//
// Can only use calls to C.addTriangle()
///
void makeSphere( Canvas &C, float radius, int slices, int stacks )
{
    // IF DOING RECURSIVE SUBDIVISION:
    //   use only the "slices" parameter
    //   use a minimum value of 1 instead of 3
    //   add an 'else' clause to set a maximum value of 5

    if( slices < 3 )
        slices = 3;

    if( stacks < 3 )
        stacks = 3;
    
    Vertex top={0, 0.5, 0}, bot={0, -0.5, 0};
    Vertex sphere[stacks+1][slices];
    double phi=0, theta=0;
    for(int i=1;i<stacks;i++){//create vertices
		phi+=PI/((float)stacks);
		for(int j=0;j<slices;j++){
			theta+=2*PI/((float)slices);
			sphere[i][j].x=radius*cos(theta)*sin(phi);
			sphere[i][j].y=radius*cos(phi);
			sphere[i][j].z=radius*sin(theta)*sin(phi);
		}
		
		theta=0;
	}
}

///
//  Viewing
//
//  Created by Warren R. Carithers 2016/11/11.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Simple class for setting up the viewing and projection transforms
//  for the Texting assignment.
//
//  This code can be compiled as either C or C++.
//
//  This file should not be modified by students.
///

#include "Viewing.h"

// current values for transformations
GLfloat rotateDefault[3]    = { 0.0f, 50.0f, 90.0f };
GLfloat translateDefault[3] = { 1.0f, 0.0f, -1.0f };
GLfloat scaleDefault[3]     = { 1.0f, 4.0f, 1.0f };

// current view values
GLfloat eyeDefault[3]  = { 0.0f, 3.0f, 3.0f };
GLfloat lookDefault[3] = { 1.0f, 0.0f, 0.0f };
GLfloat upDefault[3]   = { 0.0f, 1.0f, 0.0f };

// clipping window boundaries
GLfloat cwLeft   = -1.0f;
GLfloat cwRight  = 1.0f;
GLfloat cwTop    = 1.0f;
GLfloat cwBottom = -1.0f;
GLfloat cwNear   = 3.0f;
GLfloat cwFar    = 100.5f;

///
// This function sets up the view and projection parameters for a frustum
// projection of the scene.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpProjection( GLuint program )
{
    GLint leftLoc = glGetUniformLocation( program, "left" );
    GLint rightLoc = glGetUniformLocation( program, "right" );
    GLint topLoc = glGetUniformLocation( program, "top" );
    GLint bottomLoc = glGetUniformLocation( program, "bottom" );
    GLint nearLoc = glGetUniformLocation( program, "near" );
    GLint farLoc = glGetUniformLocation( program, "far" );

    glUniform1f( leftLoc,   cwLeft );
    glUniform1f( rightLoc,  cwRight );
    glUniform1f( topLoc,    cwTop );
    glUniform1f( bottomLoc, cwBottom );
    glUniform1f( nearLoc,   cwNear );
    glUniform1f( farLoc,    cwFar );
}

///
// This function clears any transformations, setting the values to the
// defaults: scale by 4 in Y, rotate by 50 in Y and 90 in Z, and
// translate by 1 in X and -1 in Z.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void clearTransforms( GLuint program )
{
    // reset the shader using global data
    GLint thetaLoc = glGetUniformLocation( program, "theta" );
    GLint transLoc = glGetUniformLocation( program, "trans" );
    GLint scaleLoc = glGetUniformLocation( program, "scale" );

    glUniform3fv( thetaLoc, 1, rotateDefault );
    glUniform3fv( transLoc, 1, translateDefault );
    glUniform3fv( scaleLoc, 1, scaleDefault );
}

///
// This function sets up the transformation parameters for the vertices
// of the teapot.  The order of application is specified in the driver
// program.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param scale     - scale factors for each axis
// @param rotate    - rotation angles around the three axes, in degrees
// @param translate - amount of translation along each axis
///
void setUpTransforms( GLuint program, Tuple scale, Tuple rotate, Tuple xlate )
{
    GLfloat scaleVec[]     = { scale.x, scale.y, scale.z };
    GLfloat rotateVec[]    = { rotate.x, rotate.y, rotate.z };
    GLfloat translateVec[] = { xlate.x, xlate.y, xlate.z };

    GLint thetaLoc = glGetUniformLocation( program, "theta" );
    GLint transLoc = glGetUniformLocation( program, "trans" );
    GLint scaleLoc = glGetUniformLocation( program, "scale" );

    // send down to the shader
    glUniform3fv( thetaLoc, 1, rotateVec );
    glUniform3fv( transLoc, 1, translateVec );
    glUniform3fv( scaleLoc, 1, scaleVec );
}

///
// This function clears any changes made to camera parameters, setting the
// values to the defaults: eyepoint (0.0,3.0,3.0), lookat (1,0,0.0,0.0),
// and up vector (0.0,1.0,0.0).
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void clearCamera( GLuint program )
{
    GLint posLoc = glGetUniformLocation( program, "cPosition" );
    GLint lookLoc = glGetUniformLocation( program, "cLookAt" );
    GLint upVecLoc = glGetUniformLocation( program, "cUp" );

    glUniform3fv( posLoc, 1, eyeDefault );
    glUniform3fv( lookLoc, 1, lookDefault );
    glUniform3fv( upVecLoc, 1, upDefault );
}

///
// This function sets up the camera parameters controlling the viewing
// transformation.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param eye    - camera location
// @param lookat - lookat point
// @param up     - the up vector
///
void setUpCamera( GLuint program, Tuple eye, Tuple lookat, Tuple up )
{
    GLfloat eyeVec[]    = { eye.x, eye.y, eye.z };
    GLfloat lookatVec[] = { lookat.x, lookat.y, lookat.z };
    GLfloat upVec[]     = { up.x, up.y, up.z };

    GLint posLoc = glGetUniformLocation( program, "cPosition" );
    GLint lookLoc = glGetUniformLocation( program, "cLookAt" );
    GLint upVecLoc = glGetUniformLocation( program, "cUp" );

    // send down to the shader
    glUniform3fv( posLoc, 1, eyeVec );
    glUniform3fv( lookLoc, 1, lookatVec );
    glUniform3fv( upVecLoc, 1, upVec );
}

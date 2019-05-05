///
//  Lighting
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on code created by Joe Geigel on 1/23/13.
//  Updated 2018/11/29 by wrc.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Contributor:  YOUR_NAME_NERE
//
//  Simple class for setting up Phong illumination/shading.
//
//  This code can be compiled as either C or C++.
///

#include <exception>
#include "Lighting.h"
#include "Shapes.h"
#include "WorldParameters.h"

using namespace std;

///
// This function sets up the lighting, material, and shading parameters
// for the Phong shader.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param obj - The object type of the object being drawn
///
void setUpPhong( GLuint program, int obj )
{
    GLuint locLightLocation = glGetUniformLocation(program, "lightLocation");
    GLuint locAmbientIllumination = glGetUniformLocation(program, "lightAmbientIllumination");
    GLuint locAmbientReflectivity = glGetUniformLocation(program, "lightAmbientReflectivity");
    GLuint locAmbientColor = glGetUniformLocation(program, "lightAmbientColor");
    GLuint locDiffuseIllumination = glGetUniformLocation(program, "lightDiffuseIllumination");
    GLuint locDiffuseReflectivity = glGetUniformLocation(program, "lightDiffuseReflectivity");
    GLuint locDiffuseColor = glGetUniformLocation(program, "lightDiffuseColor");
    GLuint locSpecularIllumination = glGetUniformLocation(program, "lightSpecularIllumination");
    GLuint locSpecularReflectivity = glGetUniformLocation(program, "lightSpecularReflectivity");
    GLuint locSpecularColor = glGetUniformLocation(program, "lightSpecularColor");
    GLuint locSpecularExponent = glGetUniformLocation(program, "lightSpecularExponent");

    //
    // Light source properties

    glUniform3fv(locLightLocation, 1, lightLocation);
    glUniform3fv(locAmbientIllumination, 1, ambientIllumination);
    glUniform3fv(locDiffuseIllumination, 1, diffuseIllumination);
    glUniform3fv(locSpecularIllumination, 1, specularIllumination);

    //
    // Determine which model we're rendering; send in the respective
    // material properties. See WorldParameters.h for the definition
    // of these variables.

    if (obj == OBJ_TEAPOT)
    {
        glUniform1f(locAmbientReflectivity, teapotAmbientReflectivity);
        glUniform3fv(locAmbientColor, 1, teapotAmbientColor);
        glUniform1f(locDiffuseReflectivity, teapotDiffuseReflectivity);
        glUniform3fv(locDiffuseColor, 1, teapotDiffuseColor);
        glUniform1f(locSpecularReflectivity, teapotSpecularReflectivity);
        glUniform3fv(locSpecularColor, 1, teapotSpecularColor);
        glUniform1f(locSpecularExponent, teapotSpecularExponent);
    }
    else if (obj == OBJ_SPHERE)
    {
        glUniform1f(locAmbientReflectivity, sphereAmbientReflectivity);
        glUniform3fv(locAmbientColor, 1, sphereAmbientColor);
        glUniform1f(locDiffuseReflectivity, sphereDiffuseReflectivity);
        glUniform3fv(locDiffuseColor, 1, sphereDiffuseColor);
        glUniform1f(locSpecularReflectivity, sphereSpecularReflectivity);
        glUniform3fv(locSpecularColor, 1, sphereSpecularColor);
        glUniform1f(locSpecularExponent, sphereSpecularExponent);
    }
    else
    {
        __throw_invalid_argument("Specified object obj not supported.");
    }
}

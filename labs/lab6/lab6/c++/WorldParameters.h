///
// WorldParameters.h
//
// Lighting and material parameter definitions for
// our shaders. Used by Lighting.cpp.
///

#include <GLFW/glfw3.h>

GLfloat lightLocation[] = { 3.0, 9.0, 2.0 };

GLfloat ambientIllumination[] = { 0.5, 0.5, 0.5 };
GLfloat diffuseIllumination[] = { 1.0, 1.0, 1.0 };
GLfloat specularIllumination[] = { 1.0, 1.0, 1.0 };

GLfloat teapotAmbientReflectivity = 0.5;
GLfloat teapotAmbientColor[] = { 0.5, 0.1, 0.9 };
GLfloat teapotDiffuseReflectivity = 0.7;
GLfloat teapotDiffuseColor[] = { 0.89, 0.0, 0.0 };
GLfloat teapotSpecularReflectivity = 1.0;
GLfloat teapotSpecularColor[] = { 1.0, 1.0, 1.0 };
GLfloat teapotSpecularExponent = 10.0;

GLfloat sphereAmbientReflectivity = 0.5;
GLfloat sphereAmbientColor[] = { 0.5, 0.5, 0.5 };
GLfloat sphereDiffuseReflectivity = 0.8;
GLfloat sphereDiffuseColor[] = { 0.49, 0.99, 0.0 };
GLfloat sphereSpecularReflectivity = 1.0;
GLfloat sphereSpecularColor[] = { 1.0, 1.0, 1.0 };
GLfloat sphereSpecularExponent = 50.0;
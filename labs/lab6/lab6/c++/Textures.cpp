///
//  Textures
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on code created by Joe Geigel on 1/23/13.
//  Updated 2018/11/29 by wrc.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
//
//  Simple class for setting up texture mapping parameters.
//
//  This code can be compiled as either C or C++.
///

#ifdef __cplusplus
#include <iostream>
#else
#include <stdio.h>
#endif

#include "Textures.h"
#include "Shapes.h"

// this is here in case you are using SOIL;
// if you're not, it can be deleted.
#include <SOIL.h>

#ifdef __cplusplus
using namespace std;
#endif

// Add any global definitions and/or variables you need here.
GLuint smiley2_texture;
GLuint frowny2_texture;

///
// This function loads texture data for the GPU.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the various shaders.
///
void loadTextures( void )
{
    //
    // Store first texture in texture 0

    glActiveTexture(GL_TEXTURE0);
    smiley2_texture = SOIL_load_OGL_texture(
		"../images/smiley2.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Clear out bindings per best practices
    glBindTexture(GL_TEXTURE_2D, 0);
	
    if(0 == smiley2_texture)
    {
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
    }

    //
    // Store second texture in texture 1

    glActiveTexture(GL_TEXTURE1);
    frowny2_texture = SOIL_load_OGL_texture(
		"../images/frowny2.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Clear out bindings per best practices
    glBindTexture(GL_TEXTURE_2D, 0);
	
    if(0 == frowny2_texture)
    {
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
    }
}

///
// This function sets up the parameters for texture use.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the various shaders.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param obj - The object type of the object being drawn
///
void setUpTextures( GLuint program, int obj )
{
    int smiley2TextureLocation;
    int frowny2TextureLocation;

    if (obj == OBJ_QUAD)
    {
        // Load our textures into their respective uniforms

        smiley2TextureLocation = glGetUniformLocation(program, "smiley2Texture");
        frowny2TextureLocation = glGetUniformLocation(program, "frowny2Texture");

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, smiley2_texture);
        glUniform1i(smiley2TextureLocation, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, frowny2_texture);
        glUniform1i(frowny2TextureLocation, 1);
    }
    else
    {
        // Ensure both of our used texture spaces are cleared out
        // (though technically phong.frag isn't currently accessing
        // them anyway...)

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

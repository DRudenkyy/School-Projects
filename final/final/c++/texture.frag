#version 130

// Texture mapping vertex shader
//
// Contributor:  YOUR_NAME_HERE

// INCOMING DATA

// Here is where you should add the variables you need in order
// to propogate data from the vertex shader to the fragment shader
// so that it can do the shading and texture mapping computations

in vec2 texCoord;
uniform sampler2D smiley2Texture;
uniform sampler2D frowny2Texture;

// OUTGOING DATA

out vec4 finalColor;

void main()
{
    // Replace this code with your implementation of this shader

    vec4 textureColor;
    
    if (gl_FrontFacing)
        finalColor = texture(smiley2Texture, texCoord);
    else
    {
        finalColor = texture(frowny2Texture, texCoord);
    }
}

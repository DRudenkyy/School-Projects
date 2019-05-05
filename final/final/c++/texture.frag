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

    // Having some fun below; this was not the original spec for
    // lab 6 but shows some of what's possible with more than
    // one texture. May also want to check out mix().
    
    if (texCoord.x > 0.5)
        textureColor = texture(frowny2Texture, texCoord);
    else
        textureColor = texture(smiley2Texture, texCoord);

    if (gl_FrontFacing)
        finalColor = textureColor;
    else
    {
        finalColor.rgba = textureColor.rrrr;
    }
}

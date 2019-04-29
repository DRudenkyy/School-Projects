#version 130

// Texture mapping vertex shader
//
// Contributor:  YOUR_NAME_HERE

// INCOMING DATA

// Here is where you should add the variables you need in order
// to propogate data from the vertex shader to the fragment shader
// so that it can do the shading and texture mapping computations

uniform sampler2D tex;
in vec2 texCoord;

// OUTGOING DATA

out vec4 finalColor;

void main()
{
    // Replace this code with your implementation of this shader
    
    finalColor = texture(tex, texCoord);
}

#version 130

// Phong fragment shader
//
// Contributor:  Jake Brandt

// INCOMING DATA

in vec3 lightVector;
in vec3 viewVector;
in vec3 surfaceNormal;

uniform vec3 lightAmbientIllumination;
uniform float lightAmbientReflectivity;
uniform vec3 lightAmbientColor;
uniform vec3 lightDiffuseIllumination;
uniform float lightDiffuseReflectivity;
uniform vec3 lightDiffuseColor;
uniform vec3 lightSpecularIllumination;
uniform float lightSpecularReflectivity;
uniform vec3 lightSpecularColor;
uniform float lightSpecularExponent;

// Here is where you should add the variables you need in order
// to propogate data from the vertex shader to the fragment shader
// so that it can do the shading computations

// OUTGOING DATA
out vec4 finalColor;

void main()
{
    // Replace this code with your implementation of this shader

    vec3 reflectionVector = reflect(-lightVector, surfaceNormal);

    vec3 ambient =
        lightAmbientIllumination *
        lightAmbientReflectivity *
        lightAmbientColor;

    vec3 diffuse =
        lightDiffuseIllumination *
        lightDiffuseReflectivity *
        lightDiffuseColor *
        max(dot(lightVector, surfaceNormal), 0.0);
    
    vec3 specular =
        lightSpecularIllumination *
        lightSpecularReflectivity *
        lightSpecularColor *
        pow(max(dot(reflectionVector, viewVector), 0.0), lightSpecularExponent);

    // Sum up our lighting components; however, don't let the
    // color channels exceed 1.0.
    finalColor.rgb = min(
         ambient +
         diffuse +
         specular,
         vec3(1.0, 1.0, 1.0));
    
    // Set our alpha to be fully opaque
    finalColor.a = 1.0;
}

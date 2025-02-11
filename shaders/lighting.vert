/////////////////////////////////////////////////////////////////////////
// Vertex shader for lighting
//
// Copyright 2013 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////
#version 330

uniform mat4 WorldView, WorldInverse, WorldProj, ModelTr, NormalTr;

uniform vec3 Light, Ambient;

in vec3 vertex;
in vec3 vertexNormal;
in vec2 vertexTexture;
in vec3 vertexTangent;

uniform vec2 texScale;
uniform float texRot;

const float degToRad = 0.017453292;


out vec3 tanVec; // A name of your choice


// All of these are fed directy into the in of frag shader
out vec3 normalVec, lightVec, eyeVec;

out vec2 texCoord;

uniform vec3 lightPos;

void main()
{      
    vec4 h_vertex = vec4(vertex, 1);
    // Needed to feed into the graphics pipeline
    gl_Position = WorldProj*WorldView*ModelTr*h_vertex;
    
    vec3 worldPos = (ModelTr * h_vertex).xyz;
    
    vec3 eyePos = (WorldInverse*vec4(0,0,0,1)).xyz;
    // Outputs fed into frag shader
    eyeVec = eyePos - worldPos;

    normalVec = vertexNormal*mat3(NormalTr); 
    lightVec = lightPos - worldPos;

    float theta = texRot * degToRad;

    float cs = cos(theta);
    float sn = sin(theta);

    vec2 temp = (vertexTexture * texScale);


    tanVec = mat3(ModelTr)*vertexTangent; // Proper way to transform a VECTOR
    texCoord.xy = vec2(temp.x * cs - temp.y * sn, temp.x * sn + temp.y * cs);
}

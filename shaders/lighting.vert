/////////////////////////////////////////////////////////////////////////
// Vertex shader for lighting
//
// Copyright 2013 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////
#version 330

uniform mat4 WorldView, WorldInverse, WorldProj, ModelTr, NormalTr;

uniform vec3 Light, Ambient;
uniform vec3 lightPos;

in vec3 vertex_position;
in vec3 vertex_normal;
in vec2 vertex_texture;
in vec3 vertex_tangent;

uniform vec2 texScale;
uniform float texRot;

const float degToRad = 0.017453292;

out vec3 tanVec;

// These are fed directy into the fragment shader
out vec3 normalVec, lightVec, eyeVec;
out vec2 texCoord;

void main() {
    gl_Position = WorldProj*WorldView*ModelTr*vec4(vertex_position, 1);
    
    vec3 worldPos = (ModelTr * vec4(vertex_position, 1)).xyz;
    
    vec3 eyePos = (WorldInverse*vec4(0,0,0,1)).xyz;
    eyeVec = eyePos - worldPos;

    normalVec = (vec4(vertex_normal, 1) * NormalTr).xyz; 
    lightVec = lightPos - worldPos;

    float theta = texRot * degToRad;

    float cs = cos(theta);
    float sn = sin(theta);

    vec2 temp = (vertex_texture * texScale);


    tanVec = mat3(ModelTr)*vertex_tangent;
    texCoord.xy = vec2(temp.x * cs - temp.y * sn, temp.x * sn + temp.y * cs);
}

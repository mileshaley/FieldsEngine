/////////////////////////////////////////////////////////////////////////
// Pixel shader for lighting
////////////////////////////////////////////////////////////////////////
#version 330

out vec4 FragColor;

// These definitions agree with the ObjectIds enum in scene.h
const int     nullId	= 0;
const int     skyId	= 1;
const int     seaId	= 2;
const int     groundId	= 3;
const int     roomId	= 4;
const int     boxId	= 5;
const int     frameId	= 6;
const int     lPicId	= 7;
const int     rPicId	= 8;
const int     teapotId	= 9;
const int     spheresId	= 10;
const int     floorId	= 11;

const float pi = 3.141592653;
const float pi2 = 6.283185306;
const float ipi = 0.31830988624;

// Inputs from vert shader
in vec3 normalVec, lightVec, eyeVec, tanVec;
in vec2 texCoord; // uv

uniform bool hasTexture;
uniform bool hasNormal;

uniform int objectId;

uniform vec3 diffuse;    // Kd
uniform vec3 specular;   // Ks
uniform float shininess; // Alpha exponent

uniform vec3 Light;      // Ii
uniform vec3 Ambient;    // Ia

uniform sampler2D tex;
uniform sampler2D norm;

void main()
{
    // Normal to the surface
    vec3 N = normalize(normalVec);
    // Direction of light source
    vec3 L = normalize(lightVec);
    // Direction of eye
    vec3 V = normalize(eyeVec);
    // Halfway between light and eye
    vec3 H = normalize(L+V);


    // I am aware that the normal map for the sea is just using the texture
    // I have no idea how to fix it
    

    if (hasNormal)
    {
        vec3 delta = texture(norm, texCoord).rgb;
        delta = delta * 2 - vec3(1,1,1);
        vec3 T = normalize(tanVec);
        vec3 B = normalize(cross(T, N));
        N = delta.x * T + delta.y * B + delta.z * N;
    }

    vec3 R = -(N * (2 * dot(V, N)) - V);

    float NdotL = max(dot(L,N),0.0);
    float HdotN = max(dot(H,N),0.0);
    float LdotH = max(dot(L,H),0.0);
    
    vec3 Kd = diffuse;
    vec3 Ks = specular;
    vec3 Ii = Light;
    vec3 Ia = Ambient;    

    float lighting = 1;

    if (hasTexture)
    {
        Kd = texture(tex, texCoord).rgb;
    }
    else
    {
        // A checkerboard pattern to break up large flat expanses.  Remove when using textures.
//        if (objectId==groundId || objectId==floorId)// || objectId==seaId) 
//        {
//            ivec2 uv = ivec2(floor(100.0*texCoord));
//            if ((uv[0]+uv[1])%2==0)
//            {
//                Kd *= 0.9; 
//            }
//        }
        if (objectId==lPicId)
        {
            ivec2 uv = ivec2(floor(16.0*texCoord));

            if ((uv[0]+uv[1])%2==0)
            {
                Kd *= 1.9; // + 0.7f * texCoord.x / texCoord.y; 
                if ((uv[0] * uv[1])%3==0)
                {
                    Kd *= 1.8f; 
                }
            }

        }
    }

    float alpha = shininess;

    // For Schlick Approximation
    float nLdHp1 = 1-LdotH; 
    float Fpow5Term = nLdHp1 * nLdHp1 * nLdHp1 * nLdHp1 * nLdHp1;
    // F( L, H )
    vec3 F = Ks + (vec3(1,1,1) - Ks) * Fpow5Term;
    // G( L, V, H ) / LdotN * VdotN * 4
    float G_denom = 1/(LdotH * LdotH * 4);
    // D( H )
    float D = ((alpha/pi2) + ipi) * pow(HdotN, alpha);

    // BRDF portion
    vec3 BRDF = (Kd * ipi) + (F * (G_denom * D));

    // I assume that both paths will be calculated anyway so I just do the sky/sea texture check here
    FragColor.xyz = objectId == skyId ? texture(tex, vec2(-atan(V.y,V.x)/(pi2), acos(V.z) * ipi)).xyz 
    : (objectId == seaId ? texture(tex, vec2(-atan(R.y,R.x)/(pi2), acos(R.z)* ipi)).xyz
    // Micro-facet BRDF lighting
    : (Ia * Kd) + ((Ii * NdotL) * BRDF));
}

//
//    // Normal to the surface
//    vec3 N = normalize(normalVec);
//    // Direction of light source
//    vec3 L = normalize(lightVec);
//    // Direction of eye
//    vec3 V = normalize(eyeVec);
//    // Halfway between light and eye
//    vec3 H = normalize(L+V);
//
//    float NdotL = max(dot(L,N),0.0);
//    float HdotN = max(dot(H,N),0.0);
//    float LdotH = max(dot(L,H),0.0);
//    
//    vec3 Kd = diffuse;
//    vec3 Ks = specular;
//    vec3 Ii = Light;
//    vec3 Ia = Ambient;
//
//    float alpha = shininess;
//
//    // A checkerboard pattern to break up large flat expanses.  Remove when using textures.
//    if (objectId==groundId || objectId==floorId || objectId==seaId) 
//    {
//        ivec2 uv = ivec2(floor(100.0*texCoord));
//        if ((uv[0]+uv[1])%2==0)
//        {
//            Kd *= 0.9; 
//        }
//    }
//
//    // For Schlick Approximation
//    float nLdHp1 = 1-LdotH; 
//    float Fpow5Term = nLdHp1 * nLdHp1 * nLdHp1 * nLdHp1 * nLdHp1;
//    // F( L, H )
//    vec3 F = Ks + (vec3(1,1,1) - Ks) * Fpow5Term;
//    // G( L, V, H ) / LdotN * VdotN * 4
//    float G_denom = 1/(LdotH * LdotH * 4);
//    // D( H )
//    float D = ((alpha/pi2) + ipi) * pow(HdotN, alpha);
//
//    // BRDF portion
//    vec3 BRDF = (Kd * ipi) + (F * (G_denom * D));
//
//    // New Calculation
//    // Micro-facet BRDF lighting
//    FragColor.xyz = (Ia * Kd) + ((Ii * NdotL) * BRDF);
//
//    // Old Calculation
//    //FragColor.xyz = vec3(0.5,0.5,0.5)*Kd + Kd*NdotL;
//
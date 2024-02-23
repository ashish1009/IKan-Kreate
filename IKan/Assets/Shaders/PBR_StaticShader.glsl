//
//  PBR_StaticShader.glsl
//  ikan
//
//  Created by iKan on 12/02/24.
//

// vertex Shader
#type vertex

#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in vec3 a_Binormal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
  gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

// Fragment Shader
#type fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

struct DirectionLight
{
  float enable;
  float intensity;
  
  vec3 direction;
  vec3 color;
};
uniform DirectionLight u_DirectionLight;

// Calculate the light factor
vec3 CalculateLight(vec3 radiance, vec3 L, vec3 F0)
{
  return vec3(0.0f);
}

// Calculate the Direction light
vec3 CalculateDirectionLight(vec3 F0)
{
  return vec3(0.0f);
}

void main()
{
  // Calculate reflectance at normal incidence
  // Note: If dia-electric (like plastic) use F0 of 0.04 and
  //       If it's a metal, use the albedo color as F0 (metallic workflow)
  vec3 F0 = vec3(0.04);

  // Calculate the light affect
  vec3 lightResult = CalculateDirectionLight(F0);

  // Ambient light
  vec3 ambient = vec3(0.03);

  // Final Light
  vec3 finalResult = lightResult + ambient;
  
  o_Color = vec4(finalResult, 1.0f);
}

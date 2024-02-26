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

out VS_OUT
{
  vec3 WorldPosition;
  vec3 CameraPosition;
  vec3 WorldNormals;
  vec2 TexCoord;
  vec3 TangentViewPos;
  vec3 Tangent;
  vec3 Binormal;
  mat3 TBN;
}vs_out;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat3 u_NormalMatrix;
uniform vec3 u_CameraPosition;

void main()
{
  vs_out.WorldPosition = vec3(u_Transform * vec4(a_Position, 1.0));;
  vs_out.CameraPosition = u_CameraPosition;
  vs_out.TexCoord = vec2(a_TexCoord.x, 1-a_TexCoord.y);
  vs_out.WorldNormals = u_NormalMatrix * a_Normal;

  vs_out.Tangent = u_NormalMatrix * a_Tangent;
  vs_out.Binormal = u_NormalMatrix * a_Binormal;

  vec3 N  = normalize(vs_out.WorldNormals);
  vec3 T  = normalize(vs_out.Binormal);
  vec3 B  = -normalize(vs_out.Tangent);
  vs_out.TBN = mat3(T, B, N);
  vs_out.TangentViewPos = vs_out.TBN * u_CameraPosition;

  gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

// Fragment Shader
#type fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in VS_OUT
{
  vec3 WorldPosition;
  vec3 CameraPosition;
  vec3 WorldNormals;
  vec2 TexCoord;
  vec3 TangentViewPos;
  mat3 TBN;
} vs_Input;

struct PBRParameters
{
  vec3 View;
  vec3 Normal;
};
PBRParameters m_Params;

struct DirectionLight
{
  float enable;
  float intensity;
  
  vec3 direction;
  vec3 color;
};
uniform DirectionLight u_DirectionLight;

// Texture tiling Factor
uniform float u_TilingFactor;

// Texture Uniforms
uniform sampler2D u_NormalTexture;

// Texture Toggle
uniform float u_NormalTextureToggle;

// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal
// mapping the usual way for performance anyways; I do plan make a note of this
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap(float tilingFactor, vec2 texCoords)
{
  vec3 normal = vs_Input.WorldNormals;
  if (u_NormalTextureToggle > 0.5)
  {
    normal = texture(u_NormalTexture, texCoords * tilingFactor).xyz * 2.0 - 1.0;
    normal = normalize(vs_Input.TBN * normal);
  }
  return normal;
}

// Calculate the light factor
vec3 CalculateLight(vec3 radiance, vec3 L, vec3 F0)
{
  vec3 V = m_Params.View;
  vec3 H = normalize(V + L);
  vec3 N = normalize(m_Params.Normal);

  return vec3(0.0f);
}

// Calculate the Direction light
vec3 CalculateDirectionLight(vec3 F0)
{
  if (u_DirectionLight.enable > 0.5f)
  {
    vec3 L = normalize(-u_DirectionLight.direction);
    vec3 radiance = u_DirectionLight.color * u_DirectionLight.intensity;
    return CalculateLight(radiance, L, F0);
  }
  return vec3(0.0f);
}

void main()
{
  float tilingFactor = u_TilingFactor;
  vec2 texCoords = vs_Input.TexCoord;

  m_Params.View = normalize(vs_Input.CameraPosition - vs_Input.WorldPosition);
  m_Params.Normal = getNormalFromMap(tilingFactor, texCoords);

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

//
//  PBR_StaticShader.glsl
//  ikan
//
//  Created by iKan on 09/08/23.
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
  vec3 Normal;
  vec2 TexCoord;
  mat3 WorldNormals;
  mat3 WorldTransform;
  vec3 Binormal;
} vs_Output;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

/// Vertex Main Function
void main()
{
  vs_Output.WorldPosition = vec3(u_Transform * vec4(a_Position, 1.0));
  vs_Output.Normal = mat3(u_Transform) * a_Normal;
  vs_Output.TexCoord = vec2(a_TexCoord.x, 1.0 - a_TexCoord.y);
  vs_Output.WorldNormals = mat3(u_Transform) * mat3(a_Tangent, a_Binormal, a_Normal);
  vs_Output.WorldTransform = mat3(u_Transform);
  vs_Output.Binormal = a_Binormal;
  vs_Output.TexCoord = vs_Output.TexCoord;
  
  gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

// Fragment Shader
#type fragment

#version 330 core
layout (location = 0) out vec4 o_Color;

in VS_OUT
{
  vec3 WorldPosition;
  vec3 Normal;
  vec2 TexCoord;
  mat3 WorldNormals;
  mat3 WorldTransform;
  vec3 Binormal;
} vs_Input;

// Stores the Material Property
uniform vec3 u_Material_AlbedoColor;
uniform float u_Material_Metalness;
uniform float u_Material_Roughness;

// Texture Uniforms
uniform sampler2D u_AlbedoTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_RoughnessTexture;
uniform sampler2D u_MetallicTexture;

// Texture Toggle
uniform float u_AlbedoTextureToggle;
uniform float u_NormalTextureToggle;
uniform float u_RoughnessTextureToggle;
uniform float u_MetallicTextureToggle;

// Texture tiling Factor
uniform float u_TilingFactor;

// Camera Params
uniform vec3 u_CameraPosition;

struct PBRParameters
{
  vec3 Albedo;
  vec3 Normal;
  float Metalness;
  float Roughness;

  vec3 View;
  float NdotV;  
};
PBRParameters m_Params;

vec3 CalculateLight(vec3 F0)
{
  // reflectance equation
  vec3 Lo = vec3(0.0);
  return Lo;
}

/// Fragment Main Function
void main()
{
  // TODO: Make things in vertex Shader if possible
  // Setup Material property
  float tilingFactor = u_TilingFactor;
  if (u_TilingFactor == 0.0f)
  {
    tilingFactor = 1.0f;
  }
  m_Params.Albedo    = (u_AlbedoTextureToggle > 0.5) ? texture(u_AlbedoTexture, vs_Input.TexCoord * tilingFactor).rgb * u_Material_AlbedoColor: u_Material_AlbedoColor;
  m_Params.Metalness = (u_MetallicTextureToggle > 0.5) ? texture(u_MetallicTexture, vs_Input.TexCoord * tilingFactor).r : u_Material_Metalness;
  m_Params.Roughness = (u_RoughnessTextureToggle > 0.5) ? texture(u_RoughnessTexture, vs_Input.TexCoord * tilingFactor).r : u_Material_Roughness;
  m_Params.Roughness = max(m_Params.Roughness, 0.05); // Minimum roughness of 0.05 to keep specular highlight
  
  // Normals (either from vertex or map)
  m_Params.Normal = normalize(vs_Input.Normal);
  if (u_NormalTextureToggle > 0.5)
  {
    m_Params.Normal = 2.0 * texture(u_NormalTexture, vs_Input.TexCoord * tilingFactor).rgb - 1.0f;
    m_Params.Normal = normalize(vs_Input.WorldNormals * m_Params.Normal);
  }
  
  m_Params.View = normalize(u_CameraPosition - vs_Input.WorldPosition);
  m_Params.NdotV = max(dot(m_Params.Normal, m_Params.View), 0.0f);

  // Calculate reflectance at normal incidence
  // Note: If dia-electric (like plastic) use F0 of 0.04 and
  //       If it's a metal, use the albedo color as F0 (metallic workflow)
  vec3 F0 = vec3(0.04);
  F0 = mix(F0, m_Params.Albedo, m_Params.Metalness);

  vec3 light = CalculateLight(F0);
  
  // Ambient lighting Even in dark there should be some Light
  // TODO: Replace with direction light later?
  // Change this factor with direction light
  vec3 ambient = vec3(0.1) * m_Params.Albedo;

  // Add the light in ambient
  vec3 color = ambient + light;
  
  o_Color = vec4(color, 1.0f);
}

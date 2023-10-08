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

const int MAX_LIGHTS = 10;

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

// Light Data
struct LightData
{
  float Active;
  vec3 Position;
  vec3 Radiance;
};
uniform float u_NumLights;
uniform LightData u_LightData[10];

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

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal
// mapping the usual way for performance anyways; I do plan make a note of this
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap()
{
  vec3 Q1  = dFdx(vs_Input.WorldPosition);
  vec3 Q2  = dFdy(vs_Input.WorldPosition);
  vec2 st1 = dFdx(vs_Input.TexCoord);
  vec2 st2 = dFdy(vs_Input.TexCoord);
  
  vec3 N   = normalize(vs_Input.Normal);
  vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
  vec3 B  = -normalize(cross(N, T));
  mat3 TBN = mat3(T, B, N);
  
  return normalize(TBN * m_Params.Normal);
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
  float a = roughness*roughness;
  float a2 = a*a;
  float NdotH = max(dot(N, H), 0.0);
  float NdotH2 = NdotH*NdotH;
  
  float nom   = a2;
  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
  denom = PI * denom * denom;
  
  return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
  float r = (roughness + 1.0);
  float k = (r*r) / 8.0;
  
  float nom   = NdotV;
  float denom = NdotV * (1.0 - k) + k;
  
  return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
  float NdotV = max(dot(N, V), 0.0);
  float NdotL = max(dot(N, L), 0.0);
  float ggx2 = GeometrySchlickGGX(NdotV, roughness);
  float ggx1 = GeometrySchlickGGX(NdotL, roughness);
  
  return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
  return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 CalculateLight(vec3 F0)
{
  // reflectance equation
  vec3 Lo = vec3(0.0);
  for(int i = 0; i < int(u_NumLights); ++i)
  {
    if (u_LightData[i].Active > 0.5)
    {
      // calculate per-light radiance
      vec3 L = normalize(u_LightData[i].Position - vs_Input.WorldPosition);
      vec3 V = m_Params.View;
      vec3 N = getNormalFromMap();
      vec3 H = normalize(V + L);
      
      float distance = length(u_LightData[i].Position - vs_Input.WorldPosition);
      float attenuation = 1.0 / (distance * distance);
      vec3 radiance = u_LightData[i].Radiance * 10;
      
      // Cook-Torrance BRDF
      float NDF = DistributionGGX(N, H, m_Params.Roughness);
      float G   = GeometrySmith(N, V, L, m_Params.Roughness);
      vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
      
      vec3 numerator    = NDF * G * F;
      float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
      vec3 specular = numerator / denominator;
      
      // kS is equal to Fresnel
      vec3 kS = F;
      // for energy conservation, the diffuse and specular light can't
      // be above 1.0 (unless the surface emits light); to preserve this
      // relationship the diffuse component (kD) should equal 1.0 - kS.
      vec3 kD = vec3(1.0) - kS;
      // multiply kD by the inverse metalness such that only non-metals
      // have diffuse lighting, or a linear blend if partly metal (pure metals
      // have no diffuse light).
      kD *= 1.0 - m_Params.Metalness;
      
      // scale light by NdotL
      float NdotL = max(dot(N, L), 0.0);
      
      // add to outgoing radiance Lo
      Lo += (kD * m_Params.Albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }
  }
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

  vec3 light = vec3(0.0f); //CalculateLight(F0);
  
  // Ambient lighting Even in dark there should be some Light
  // TODO: Replace with direction light later?
  // Change this factor with direction light
  vec3 ambient = vec3(0.1) * m_Params.Albedo;

  // Add the light in ambient
  vec3 color = ambient + light;
  
  o_Color = vec4(color, 1.0f);
}

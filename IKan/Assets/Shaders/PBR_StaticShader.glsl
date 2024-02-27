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

const float PI = 3.14159265359;

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
  vec3 Albedo;
  vec3 Normal;
  float Metalness;
  float Roughness;
  float Ao;
  vec3 View;
  float NdotV;
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

struct Material
{
  vec3 albedoColor;
  float metallic;
  float roughness;
  float depthScale;
  float tilingFactor;
};
uniform Material u_Material;

// Texture Uniforms
uniform sampler2D u_AlbedoTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_RoughnessTexture;
uniform sampler2D u_MetallicTexture;
uniform sampler2D u_DepthTexture;
uniform sampler2D u_AoTexture;

// Texture Toggle
uniform float u_AlbedoTextureToggle;
uniform float u_NormalTextureToggle;
uniform float u_RoughnessTextureToggle;
uniform float u_MetallicTextureToggle;
uniform float u_DepthTextureToggle;
uniform float u_AoTextureToggle;

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
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
  float a = roughness * roughness;
  float a2 = a*a;
  float NdotH = max(dot(N, H), 0.0);
  float NdotH2 = NdotH*NdotH;
  
  float nom   = a2;
  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
  denom = PI * denom * denom;
  
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

// Calculate the light factor
vec3 CalculateLight(vec3 radiance, vec3 L, vec3 F0)
{
  vec3 V = m_Params.View;
  vec3 H = normalize(V + L);
  vec3 N = normalize(m_Params.Normal);

  // Cook-Torrance BRDF
  float NDF = DistributionGGX(N, H, m_Params.Roughness);
  float G   = GeometrySmith(N, V, L, m_Params.Roughness);
  vec3  F   = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

  vec3  numerator   = NDF * G * F;
  float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
  vec3  specular    = numerator / denominator;

  // kS is equal to Fresnel
  vec3 kS = F;
  // for energy conservation, the diffuse and specular light can't be above 1.0 (unless the surface emits light);
  // to preserve this relationship the diffuse component (kD) should equal 1.0 - kS.
  vec3 kD = vec3(1.0) - kS;
  // multiply kD by the inverse metalness such that only non-metals have diffuse lighting, or a linear blend if
  // partly metal (pure metals have no diffuse light).
  kD *= 1.0 - m_Params.Metalness;

  // scale light by NdotL
  float NdotL = max(dot(N, L), 0.0);
  
  // add to outgoing radiance Lo
  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
  return (kD * m_Params.Albedo / PI + specular) * radiance * NdotL;
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
  float tilingFactor = u_Material.tilingFactor;
  vec2 texCoords = vs_Input.TexCoord;

  m_Params.Albedo       = (u_AlbedoTextureToggle > 0.5) ? texture(u_AlbedoTexture, texCoords * tilingFactor).rgb * u_Material.albedoColor : u_Material.albedoColor;
  m_Params.Metalness    = (u_MetallicTextureToggle > 0.5) ? texture(u_MetallicTexture, texCoords * tilingFactor).r : u_Material.metallic;
  m_Params.Roughness    = (u_RoughnessTextureToggle > 0.5) ? texture(u_RoughnessTexture, texCoords * tilingFactor).r : u_Material.roughness;
  m_Params.Ao           = (u_AoTextureToggle > 0.5) ? texture(u_AoTexture, texCoords * tilingFactor).r : 1.0f;
  m_Params.View         = normalize(vs_Input.CameraPosition - vs_Input.WorldPosition);
  m_Params.Normal       = getNormalFromMap(tilingFactor, texCoords);

  // Calculate reflectance at normal incidence
  // Note: If dia-electric (like plastic) use F0 of 0.04 and
  //       If it's a metal, use the albedo color as F0 (metallic workflow)
  vec3 F0 = vec3(0.04);
  F0 = mix(F0, m_Params.Albedo, m_Params.Metalness);

  // Calculate the light affect
  vec3 lightResult = CalculateDirectionLight(F0);

  // Ambient light
  vec3 ambient = vec3(0.03);

  // Final Light
  vec3 finalResult = lightResult + ambient;
  
  o_Color = vec4(finalResult, 1.0f);
}

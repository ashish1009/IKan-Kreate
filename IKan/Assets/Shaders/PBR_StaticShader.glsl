//
//  PBR_StaticShader.glsl
//  ikan
//
//  Created by iKan on 15/07/24.
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

void main()
{
  o_Color = vec4(1.0f);
}

//
//  MeshStencilShader.glsl
//  ikan
//
//  Created by iKan on 09/09/23.
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

/// Fragment Main Function
void main()
{
  o_Color = vec4(1.0, 0.65, 0.01, 1.0);
}

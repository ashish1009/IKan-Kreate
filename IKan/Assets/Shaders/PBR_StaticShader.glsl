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

void main()
{
  o_Color = vec4(0.2, 0.2, 0.2, 1.0f);
}

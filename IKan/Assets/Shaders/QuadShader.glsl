//
//  QuadShader.glsl
//  ikan
//
//  Created by iKan on 28/07/23.
//

// vertex Shader
#type vertex
#version 330 core

layout(location = 0) in vec3  a_Position;

uniform mat4 u_ViewProjection;

void main()
{
  gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

// Fragment Shader
#type fragment
#version 330 core
layout(location = 0) out vec4 o_Color;
layout(location = 1) out int  o_IDBuffer;

void main()
{
  vec4 texColor = vec4(1.0f);
  if(texColor.a < 0.1)
  {
    discard;
  }
  o_Color = texColor;
  o_IDBuffer = int(-1);
}

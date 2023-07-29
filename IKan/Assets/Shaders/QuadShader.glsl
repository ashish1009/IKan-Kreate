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
layout(location = 1) in vec4  a_Color;
layout(location = 2) in vec2  a_TexCoord;

uniform mat4 u_ViewProjection;

out VS_OUT
{
  vec4  Color;
  vec2  TexCoord;
} vs_out;

void main()
{
  vs_out.Color         = a_Color;
  vs_out.TexCoord      = a_TexCoord;

  gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

// Fragment Shader
#type fragment
#version 330 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int  o_IDBuffer;

in VS_OUT
{
  vec4  Color;
  vec2  TexCoord;
} fs_in;

uniform sampler2D u_Textures[16];

void main()
{
  vec4 texColor = fs_in.Color;
  texColor *= texture(u_Textures[0], fs_in.TexCoord);

  if(texColor.a < 0.1)
  {
    discard;
  }
  o_Color = texColor;
  o_IDBuffer = int(-1);
}

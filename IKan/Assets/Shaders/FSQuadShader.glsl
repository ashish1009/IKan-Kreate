//
//  FSQuadShader.glsl
//  ikan
//
//  Created by iKan on 15/07/24.
//

// vertex Shader
#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out VS_OUT
{
  vec2 TexCoord;
} vs_out;

void main()
{
  vs_out.TexCoord = a_TexCoord;
  gl_Position = vec4(a_Position, 1.0);
}

// Fragment Shader
#type fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in VS_OUT
{
  vec2 TexCoord;
} fs_in;

uniform sampler2D u_Texture;

void main()
{
  vec4 texColor = texture(u_Texture, fs_in.TexCoord);
  
  if(texColor.a < 0.1)
  {
    discard;
  }
  
  o_Color = texColor;
}

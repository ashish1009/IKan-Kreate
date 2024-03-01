//
//  HdrBloomShader.glsl
//  ikan
//
//  Created by iKan on 01/03/24.
//

// Vertex Shader -------------------------------------------------------------------------------------
#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoords;

out vec2 v_TexCoords;

void main()
{
  v_TexCoords = a_TexCoords;
  gl_Position = vec4(a_Position, 1.0);
}

// Fragment Shader ----------------------------------------------------------------------------------
#type fragment
#version 330 core

out vec4 o_Color;

in vec2 v_TexCoords;

uniform sampler2D u_HdrBuffer;

void main()
{
  vec3 hdrColor = texture(u_HdrBuffer, v_TexCoords).rgb;
  vec3 result = hdrColor;
  o_Color = vec4(result, 1.0);
}

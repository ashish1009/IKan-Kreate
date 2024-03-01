//
//  SkymapShader.glsl
//  IKan
//
//  Created by iKan on 01/03/24.
//

// vertex Shader
#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_Projection;
uniform mat4 u_RotateView;

out vec3 WorldPos;

void main()
{
  WorldPos = aPos;
  
  vec4 clipPos = u_Projection * u_RotateView * vec4(WorldPos, 1.0);
  
  gl_Position = clipPos.xyww;
}

// Fragment Shader
#type fragment
#version 330 core
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube u_EnvironmentMap;

void main()
{
  vec3 envColor = textureLod(u_EnvironmentMap, WorldPos, 0.0).rgb;
  
  // HDR tonemap and gamma correct
  envColor = envColor / (envColor + vec3(1.0));
  envColor = pow(envColor, vec3(1.0/2.2));
  
  FragColor = vec4(envColor, 1.0);
}

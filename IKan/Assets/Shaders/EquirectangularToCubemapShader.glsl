//
//  EquirectangularToCubemapShader.glsl
//  IKan
//
//  Created by iKan on 01/03/24.
//

// vertex Shader
#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 WorldPos;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
  WorldPos = aPos;
  gl_Position =  u_Projection * u_View * vec4(WorldPos, 1.0);
}

// Fragment Shader
#type fragment
#version 330 core
out vec4 FragColor;
in vec3 WorldPos;

uniform sampler2D u_EquirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
  vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
  uv *= invAtan;
  uv += 0.5;
  return uv;
}

void main()
{
  vec2 uv = SampleSphericalMap(normalize(WorldPos));
  vec3 color = texture(u_EquirectangularMap, uv).rgb;
  
  FragColor = vec4(color, 1.0);
}

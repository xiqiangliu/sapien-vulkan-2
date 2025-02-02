#version 450 

layout(set = 0, binding = 0) uniform sampler2D samplerLighting;

layout(location = 0) in vec2 inUV;
layout(location = 0) out vec4 outColor;

void main() {
  outColor = texture(samplerLighting, inUV);
  outColor = pow(outColor, vec4(1/2.2, 1/2.2, 1/2.2, 1));
}

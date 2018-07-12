#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inPos;

layout (binding = 0) uniform UBO
{
	mat4 wvp;
} ubo;

out gl_PerVertex 
{
    vec4 gl_Position;
};

void main() 
{
    vec4 pos = ubo.wvp * vec4(inPos, 1.0);
    gl_Position = pos;
}
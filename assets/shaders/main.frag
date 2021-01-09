#version 330 core
#extension GL_ARB_separate_shader_objects : require

layout(location = 2) in vec4 inColor;

out vec4 outColor;

void main()
{
    outColor = inColor;
}
#version 330 core
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_separate_shader_objects : require

layout(location = 0) in vec3 inPosition;
layout(location = 2) in vec4 inColor;
layout(location = 8) in mat4 inModel;

layout(location = 2) out vec4 outColor;

layout(location = 2) uniform mat4 uView;
layout(location = 3) uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * uView * inModel * vec4(inPosition, 1.0);
    outColor = inColor;
}

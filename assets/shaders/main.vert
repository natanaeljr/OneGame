#version 330 core
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

layout(location = 0) in vec3 position;

out vec4 color;

layout(location = 1) uniform mat4 model;
layout(location = 2) uniform mat4 view;
layout(location = 3) uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    color = gl_Position;
}
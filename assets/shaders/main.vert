#version 330 core
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

layout(location = 0) in vec3 position;

out vec4 color;

layout(location = 4) uniform mat4 modelview;

void main()
{
    gl_Position = modelview * vec4(position, 1.0);
    color = gl_Position;
}
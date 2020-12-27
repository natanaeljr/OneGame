#version 330 core

layout(location = 0) in vec3 position;

out vec4 color;

uniform mat4 modelview;

void main()
{
    gl_Position = modelview * vec4(position, 1.0);
    color = gl_Position;
}
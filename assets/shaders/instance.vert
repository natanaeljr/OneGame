#version 330 core

in vec3 aPosition;
in vec4 aColor;
in mat4 aModel;

out vec4 oColor;

uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * uView * aModel * vec4(aPosition, 1.0);
    oColor = aColor;
}

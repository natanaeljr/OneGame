#version 330 core
in vec3 aPosition;
in vec4 aColor;
out vec4 fColor;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
    fColor = aColor;
}
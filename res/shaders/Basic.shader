#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textCoord;

out vec2 vTextCoord;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
    vTextCoord = textCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 vTextCoord;

uniform vec4 uColor;
uniform sampler2D uTexture;

void main()
{
    vec4 textColor = texture(uTexture, vTextCoord);
    color = textColor;
}
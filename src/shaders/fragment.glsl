#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D textureEZ;

void main()
{
   FragColor = texture(textureEZ, TexCoord);
}
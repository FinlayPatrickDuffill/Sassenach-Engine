#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.00);
	vec4 texColor = texture(texture1, TexCoord);
    if(texColor.a < 0.1)
    discard;
}
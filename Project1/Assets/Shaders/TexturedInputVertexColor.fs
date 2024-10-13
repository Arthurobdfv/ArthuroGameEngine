#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoord;

uniform float mixRate;
uniform float numberOfFaces;

uniform sampler2D ourTexture;
uniform sampler2D ourSecondTexture;
void main()
{
	FragColor =  mix(texture(ourTexture, texCoord) * vec4(vertexColor,1.0), texture(ourSecondTexture,vec2(texCoord.x/numberOfFaces,(1-texCoord.y)/numberOfFaces)), mixRate);
};
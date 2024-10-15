#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;


uniform float mixRate;
uniform float numberOfFaces;
uniform vec4 ambientLightData;
uniform sampler2D ourTexture;
uniform sampler2D ourSecondTexture;
uniform vec4 mainLightColor;
uniform vec4 mainLightDirection;
void main()
{

	//FragColor = vec4(mainLightDirection.rgb,1.0);
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, mainLightDirection.xyz), 0.0);
	vec3 ambient = (ambientLightData.rgb/256 * ambientLightData.w); 
	vec3 diffuse = diff * (mainLightColor.xyz/256);
	vec4 partialColor =  mix(texture(ourTexture, texCoord) * vec4(vertexColor,1.0), texture(ourSecondTexture,vec2(texCoord.x/numberOfFaces,(1-texCoord.y)/numberOfFaces)), mixRate);
	FragColor = vec4((ambient + diffuse) * partialColor.xyz, 1.0);
	//FragColor =  vec3(1);
	//FragColor =	vec4(normals, 1);
};
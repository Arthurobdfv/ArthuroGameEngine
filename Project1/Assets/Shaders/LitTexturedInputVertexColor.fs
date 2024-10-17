#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

struct LightData {
	vec3 orientation;
	vec4 color;
	int lightType;
};

uniform int lightCount;
uniform LightData[8] lightData;
uniform float mixRate;
uniform float numberOfFaces;
uniform sampler2D ourTexture;
uniform sampler2D ourSecondTexture;

void main()
{

	//FragColor = vec4(mainLightDirection.rgb,1.0);
	vec3 ambient = vec3(0);
	vec3 directional = vec3(0);
	vec3 norm = normalize(Normal);
	vec3 lightDir = vec3(0);
	vec3 pointLights = vec3(0);	
for(int i =0; i< lightCount; i++){
		if(lightData[i].lightType == 2){
			ambient = lightData[i].color.rgb * lightData[i].color.w;
		}
		if(lightData[i].lightType == 0){
			directional = dot(normalize(lightData[i].orientation),norm) * lightData[i].color.w * lightData[i].color.rgb;
		}
		if(lightData[i].lightType == 1){
			pointLights += dot(normalize(lightData[i].orientation - FragPos),norm) * lightData[i].color.w * lightData[i].color.rgb;
		}
	}
	vec4 partialColor =  mix(texture(ourTexture, texCoord) * vec4(vertexColor,1.0), texture(ourSecondTexture,vec2(texCoord.x/numberOfFaces,(1-texCoord.y)/numberOfFaces)), mixRate);
	FragColor = vec4((ambient + directional) * partialColor.xyz + pointLights, 1.0);
};
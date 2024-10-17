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

uniform vec3 cameraPos;
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
	vec3 specular = vec3(0);
	vec3 viewDir = normalize(cameraPos - FragPos);
	int specPotency = 256;
	float shininess = 0.5;
for(int i =0; i< lightCount; i++){
		if(lightData[i].lightType == 2){
			ambient = lightData[i].color.rgb * lightData[i].color.w;
		}
		if(lightData[i].lightType == 0){
			vec3 normalizedLightDir = normalize(lightData[i].orientation);
			directional = dot(normalizedLightDir,norm) * lightData[i].color.w * lightData[i].color.rgb;
			
			vec3 reflectDir = reflect(normalizedLightDir, norm);
			float specIntensity = pow(max(dot(viewDir, reflectDir), 0.0), specPotency);
			specular += specIntensity * lightData[i].color.rgb * shininess;		
		}
		if(lightData[i].lightType == 1){
			vec3 normalizedLightDir = normalize(lightData[i].orientation - FragPos);	
			pointLights += dot(normalizedLightDir,norm) * lightData[i].color.w * lightData[i].color.rgb;
			
			vec3 reflectDir = reflect(-normalizedLightDir, norm);
			float specIntensity = pow(max(dot(viewDir, reflectDir), 0.0), specPotency);			
			specular += specIntensity * lightData[i].color.rgb * shininess;		
		}
	}
	vec4 partialColor =  mix(texture(ourTexture, texCoord) * vec4(vertexColor,1.0), texture(ourSecondTexture,vec2(texCoord.x/numberOfFaces,(1-texCoord.y)/numberOfFaces)), mixRate);
	FragColor = vec4((ambient + directional + specular) * partialColor.xyz + pointLights, 1.0);
};
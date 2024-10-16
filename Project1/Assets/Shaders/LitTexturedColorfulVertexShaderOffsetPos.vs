#version 330 core
	layout (location = 0) in vec3 aPos;
	//layout (location = 1) in vec3 aColor;
	layout (location = 2) in vec2 atexCoord;
	layout (location = 3) in vec3 aNormal;
	uniform vec4 offsetPos;
	out vec3 vertexColor;
	out vec2 texCoord;
	out vec3 Normal;
	out vec3 FragPos;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;
	vertexColor = vec3(1,1,1);
	texCoord = atexCoord;
};
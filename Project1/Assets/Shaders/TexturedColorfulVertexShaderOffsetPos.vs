#version 330 core

	layout (location = 0) in vec3 aPos;
	//layout (location = 1) in vec3 aColor;
	layout (location = 2) in vec2 atexCoord;
	uniform vec4 offsetPos;
	uniform mat4 transform;
	out vec3 vertexColor;
	out vec2 texCoord;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vertexColor = vec3(1,1,1);
	texCoord = atexCoord;
};
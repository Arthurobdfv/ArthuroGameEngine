
// GLAD needs to be imported before glfw
#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "EBOMeshRenderer.h"


using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void setupHelloTriangle(unsigned int* vbo, unsigned int* vao);
void setupBaseVertexShader(unsigned int* vertexShader);
void setupBaseFragShader(unsigned int* fragShader);
bool compileLinkProgram(unsigned int programId);
void setupHelloRectangleEBO(unsigned int* ebo, unsigned int* vao);
void renderRectangle(unsigned int* vbo);
void renderTriangle(unsigned int* vbo);


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";




const float triangleVerts[] = {
		-0.5f,-0.5f, 0.0f,
		 0.5f,-0.5f, 0.0f,
		 0.0f, 0.5f, 0.0f,
};

float rectangleVerts[] = {
	0.5f, 0.5f, 0.0f, // top right
	0.5f,-0.5f, 0.0f, // bottom right
	-0.5f,-0.5f, 0.0f, // bottom left
	-0.5f, 0.5f, 0.0f, // top left
	0.0f, 0.75f, 0.0f
};
unsigned int indexes[] = {
	0,1,3,
	1,2,3,
	3,4,0
};


int main() {
	// Vert Data for initial triangle
	float* vertDataPtr = rectangleVerts;
	MeshRenderer* mesh = new EBOMeshRenderer(vertDataPtr, sizeof(rectangleVerts) / sizeof(float), &indexes[0], sizeof(indexes) / sizeof(unsigned int));
	void framebuffer_size_callback(GLFWwindow * window, int width, int height);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Game Engine", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Vertex Buffer Object id
	unsigned int vertexShaderId;
	unsigned int fragShaderId;

	unsigned int VBO =0;
	unsigned int VAO = 0;
	setupBaseVertexShader(&vertexShaderId);
	setupBaseFragShader(&fragShaderId);

	

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShaderId);
	glAttachShader(shaderProgram, fragShaderId);
	glLinkProgram(shaderProgram);
	if (compileLinkProgram(shaderProgram)) {
		// For some reason we can delete the shaders after linking?...
		// Need to understand better the glLinkProgram;
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragShaderId);



		// 0. copy our vertices array in a buffer for OpenGL to use

		//setupHelloRectangleEBO(&VBO);



		// Specified how the vertex shader should read the input data.... 
		// We need to make sure the translation of the vertex data follows a convention in this "Engine"
		// This will read the data from the last VBO bound to GL_ARRAY_BUFFER
		//setupHelloTriangle(&VBO, &VAO);
		//setupHelloRectangleEBO(&VBO, &VAO);
		mesh->SetupMesh();

		glVertexAttribPointer(
			0, // Matching layout index of the attribute we want to set 
			3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	while (!glfwWindowShouldClose(window)) {
		// processing inputs
		processInput(window);

		// rendernig commands
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		
		
		// We are using EBOs



		// Triangles when using Vertex Array Object (VAO)
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//renderTriangle(&VAO);
		//renderRectangle(&VAO);
		mesh->RenderMesh();
		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void setupHelloRectangleEBO(unsigned int* vbo, unsigned int* vao) {
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glGenBuffers(1, vbo);
	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVerts), rectangleVerts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
}
void renderRectangle(unsigned int* vbo) {
	glBindVertexArray(*vbo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void renderTriangle(unsigned int* vbo) {
	glBindVertexArray(*vbo);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void setupHelloTriangle(unsigned int* vbo, unsigned int* vao) {
	glGenBuffers(1, vbo);
	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVerts), triangleVerts, GL_STATIC_DRAW);

}

bool compileShader(unsigned int shaderId) {
	glCompileShader(shaderId);
	int success;
	char infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		cout << "Shader compilation failed!\n" << infoLog << endl;
	}
	else
	{
		cout << "Compilation success for shader: " << shaderId << endl;
	}
	return success;
}

bool compileLinkProgram(unsigned int programId) {
	int success;
	char infoLog[512];
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
		cout << "Link compilation failed!\n" << infoLog << endl;
	else
		cout << "Linked program with Id " << programId << " successfully!" << endl;
	return success;
}

void setupBaseVertexShader(unsigned int* vertexShader) {
	*vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(*vertexShader, 1, &vertexShaderSource, NULL);
	compileShader(*vertexShader);
}

void setupBaseFragShader(unsigned int* fragShader) {
	*fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(*fragShader, 1, &fragShaderSource, NULL);
	compileShader(*fragShader);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


// GLAD needs to be imported before glfw
#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "EBOMeshRenderer.h"
#include <thread>
#include "Shader.h"

using namespace std;

#pragma region AppConfig Struct
struct AppConfig {
	public:
		bool* renderWireframe = &renderWf;
		bool* closeApp = &close;

	private:
		bool renderWf = false;
		bool close = false;
};
#pragma endregion

// Move this to respective classes in future
#pragma region Function Headers
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void setupHelloTriangle(unsigned int* vbo, unsigned int* vao);
void setupBaseVertexShader(unsigned int* vertexShader, char* shaderContent);
void setupBaseFragShader(unsigned int* fragShader, char* shaderContent);
bool compileLinkProgram(unsigned int programId);
void setupHelloRectangleEBO(unsigned int* ebo, unsigned int* vao);
void renderRectangle(unsigned int* vbo);
void renderTriangle(unsigned int* vbo);
void inputLoop(AppConfig* appCfg, bool* threadFinished);
#pragma endregion

#pragma region Vertex Hardcoded Data
const float triangleVerts[] = {
	// positions	 // colors
	 0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
	 -0.5f,-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
	 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f // top
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
#pragma endregion

int main() {
	// Vert Data for initial triangle
	AppConfig* configs = new AppConfig();
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

	Shader darkRedTriangleShader = Shader("DarkRedVertexColor.vs", "InputVertexColor.fs");
	Shader colorfulVertexTriangleShader = Shader("ColorfulVertexShaderOffsetPos.vs", "InputVertexColor.fs");
	unsigned int VBO =0;
	unsigned int VAO = 0;

	setupHelloTriangle(&VBO, &VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


	float offsetX = 0, offsetY = 0;
	bool threadClosed = false;
	thread newThread(inputLoop, configs, &threadClosed);
	while (!glfwWindowShouldClose(window) && !*configs->closeApp && !threadClosed) {
		// processing inputs
		processInput(window);

		if (glfwGetKey(window, GLFW_KEY_W))
			offsetY += 0.05;
		else if (glfwGetKey(window, GLFW_KEY_S))
			offsetY -= 0.05;

		if (glfwGetKey(window, GLFW_KEY_A))
			offsetX -= 0.05;
		else if (glfwGetKey(window, GLFW_KEY_D))
			offsetX += 0.05;

		// rendernig commands
		glClear(GL_COLOR_BUFFER_BIT);
		colorfulVertexTriangleShader.Use();
		colorfulVertexTriangleShader.SetVector("offsetPos", offsetX, offsetY);
		renderTriangle(&VAO);

		//mesh->RenderMesh(configs->renderWireframe);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	newThread.join();
	glfwTerminate();
	return 0;
}

void inputLoop(AppConfig* appCfg, bool* threadFinished) {
	*threadFinished = false;
	while(!*appCfg->closeApp){
		string command;
		cin >> command;
		if (command._Equal("wf")) {
			bool renderWfValue = !(*appCfg->renderWireframe);
			*appCfg->renderWireframe = renderWfValue;
		}
		else if (command._Equal("exit"))
			*appCfg->closeApp = !*appCfg->closeApp;
	}
	*threadFinished = true;
}


#pragma region Function Implementations
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
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
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
	if (!success) {
		glGetProgramInfoLog(programId, 512, NULL, infoLog);
		cout << "Link compilation failed!\n" << infoLog << endl;
	}
	else
		cout << "Linked program with Id " << programId << " successfully!" << endl;
	return success;
}

void setupBaseVertexShader(unsigned int* vertexShader, char* shaderContent) {
	*vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(*vertexShader, 1, &shaderContent, NULL);
	compileShader(*vertexShader);
}

void setupBaseFragShader(unsigned int* fragShader, char* shaderContent) {
	*fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(*fragShader, 1, &shaderContent, NULL);
	compileShader(*fragShader);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
#pragma endregion

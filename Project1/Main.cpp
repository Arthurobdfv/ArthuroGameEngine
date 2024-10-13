
// GLAD needs to be imported before glfw
#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "EBOMeshRenderer.h"
#include <thread>
#include "Shader.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


using namespace std;
using namespace glm;

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
void setupHelloRectangleEBO(unsigned int* ebo, unsigned int* vao);
void renderRectangle(unsigned int* vbo);
void renderTriangle(unsigned int* vbo);
void inputLoop(AppConfig* appCfg, bool* threadFinished);
unsigned int LoadImages(string source, int type);
#pragma endregion

#pragma region Vertex Hardcoded Data
const float triangleVerts[] = {
	// positions	 // colors			// Tex coords
	 0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f,		// bottom right
	 -0.5f,-0.5f, 0.0f, 0.0f, 1.0f, 0.0f,		// bottom left
	 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f // top
};

float rectangleVerts[] = {
	// Positions		// Colors			//Tex Coords
	0.5f, 0.5f, 0.0f,	1.0f,0.0f,0.0f,		1.0f,1.0f,		// top right
	0.5f,-0.5f, 0.0f,	0.0f,1.0f,0.0f,		1.0f,0.0f,		// bottom right
	-0.5f,-0.5f, 0.0f,	0.0f,0.0f,1.0f,		0.0f,0.0f,		// bottom left
	-0.5f, 0.5f, 0.0f,	1.0f,1.0f,0.0f,		0.0f,1.0f,		// top left
};
unsigned int indexes[] = {
	0,1,3,
	1,2,3,
	3,4,0
};

float texCoords[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.5f, 1.0f
};
#pragma endregion

int main() {

	vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	mat4 trans = mat4(1.0f);
	//trans = translate(trans, vec3(1.0f, 1.0f, 0.0f));
	//trans = rotate(trans, radians(90.0f), vec3(0.0, 0.0, 1.0));
	//trans = scale(trans, vec3(0.5, 0.5, 0.5));
	vec = trans * vec;
	cout << vec.x << " ," << vec.y << " ," << vec.z << endl;


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
	Shader textureAndVertexColor = Shader("./Assets/Shaders/TexturedColorfulVertexShaderOffsetPos.vs", "./Assets/Shaders/TexturedInputVertexColor.fs");
	unsigned int VBO = 0;
	unsigned int VAO = 0;

	setupHelloRectangleEBO(&VBO, &VAO);
	unsigned int brickWallTexId = LoadImages("./Assets/Images/brick_wall.jpg", GL_RGB);
	unsigned int emojiTexId = LoadImages("./Assets/Images/emoji.png", GL_RGBA);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickWallTexId);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, emojiTexId);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int transformLoc = glGetUniformLocation(textureAndVertexColor.programId, "transform");
	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));

	float offsetX = 0, offsetY = 0, mixRate = 0, numberOfFaces = 1;
	bool threadClosed = false;
	thread newThread(inputLoop, configs, &threadClosed);
	while (!glfwWindowShouldClose(window) && !*configs->closeApp) {
		// processing inputs
		processInput(window);

		// Small WASD command to play around with the mesh, will not look like this in the end
		if (glfwGetKey(window, GLFW_KEY_W))
			offsetY += 0.05;
		else if (glfwGetKey(window, GLFW_KEY_S))
			offsetY -= 0.05;

		if (glfwGetKey(window, GLFW_KEY_A))
			offsetX -= 0.05;
		else if (glfwGetKey(window, GLFW_KEY_D))
			offsetX += 0.05;

		if (glfwGetKey(window, GLFW_KEY_UP))
			mixRate += 0.05;
		else if (glfwGetKey(window, GLFW_KEY_DOWN))
			mixRate -= 0.05;

		if (glfwGetKey(window, GLFW_KEY_RIGHT))
			numberOfFaces += 0.05;
		else if (glfwGetKey(window, GLFW_KEY_LEFT))
			numberOfFaces -= 0.05;

		if (mixRate > 1)
			mixRate = 1;
		if (mixRate < 0)
			mixRate = 0;

		if (numberOfFaces <= 0)
			numberOfFaces = 0.05;


		// rendernig commands
		glClear(GL_COLOR_BUFFER_BIT);
		textureAndVertexColor.Use();
		textureAndVertexColor.SetInt("ourTexture", 0);
		textureAndVertexColor.SetInt("ourSecondTexture", 1);
		textureAndVertexColor.SetFloat("mixRate", mixRate);
		textureAndVertexColor.SetFloat("numberOfFaces", numberOfFaces);
		textureAndVertexColor.SetVector("offsetPos", offsetX, offsetY);
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		trans = glm::rotate(trans, (float)glfwGetTime(),
			glm::vec3(0.0f, 0.0f, 1.0f));
		//glBindTexture(GL_TEXTURE_2D, texId);
		renderRectangle(&VAO);
		//renderTriangle(&VAO);

		//mesh->RenderMesh(configs->renderWireframe);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	newThread.join();
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

unsigned int LoadImages(string source, int type) {
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(source.c_str(), &width, &height, &nrChannels, 0);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, type, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	return texture;
}
#pragma endregion

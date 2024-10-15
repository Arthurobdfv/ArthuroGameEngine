
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
#include "Components/Transform/TransformComponent.h"
#include "Components/GameObject/GameObject.h"
#include "./Core/Rendering/MeshRenderer/VAOMeshRenderer/VAOMeshRenderer.h"


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
void renderTriangle(unsigned int* vbo, unsigned int vertexCount);
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

float cubeVertices[] = {
	-0.5f,-0.5f,-0.5f,		0.0f, 0.0f,
	 0.5f,-0.5f,-0.5f,		1.0f, 0.0f,
	 0.5f, 0.5f,-0.5f,		1.0f, 1.0f,
	 0.5f, 0.5f,-0.5f,		1.0f, 1.0f,
	-0.5f, 0.5f,-0.5f,		0.0f,1.0f,
	-0.5f,-0.5f,-0.5f,		0.0f,0.0f,
	-0.5f,-0.5f, 0.5f,		0.0f,0.0f,
	0.5f,-0.5f, 0.5f,		1.0f,0.0f,
	0.5f, 0.5f, 0.5f,		1.0f,1.0f,
	0.5f, 0.5f, 0.5f,		1.0f,1.0f,
	-0.5f, 0.5f, 0.5f,		0.0f,1.0f,
	-0.5f,-0.5f, 0.5f,		0.0f,0.0f
	,-0.5f, 0.5f, 0.5f,		1.0f,0.0f,
	-0.5f, 0.5f,-0.5f,		1.0f,1.0f,
	-0.5f,-0.5f,-0.5f,		0.0f,1.0f,
	-0.5f,-0.5f,-0.5f,		0.0f,1.0f,
	-0.5f,-0.5f, 0.5f,		0.0f,0.0f,
	-0.5f, 0.5f, 0.5f,		1.0f,0.0f,
	 0.5f, 0.5f, 0.5f,		1.0f,0.0f,
	 0.5f, 0.5f,-0.5f,		1.0f,1.0f,
	 0.5f,-0.5f,-0.5f,		0.0f,1.0f,
	 0.5f,-0.5f,-0.5f,		0.0f,1.0f,
	 0.5f,-0.5f, 0.5f,		0.0f,0.0f,
	 0.5f, 0.5f, 0.5f,		1.0f,0.0f,
	-0.5f,-0.5f,-0.5f,		0.0f,1.0f,
	 0.5f,-0.5f,-0.5f,		1.0f,1.0f,
	 0.5f,-0.5f, 0.5f,		1.0f,0.0f,
	 0.5f,-0.5f, 0.5f,		1.0f,0.0f,
	-0.5f,-0.5f, 0.5f,		0.0f,0.0f,
	-0.5f,-0.5f,-0.5f,		0.0f,1.0f,
	-0.5f, 0.5f,-0.5f,		0.0f,1.0f,
	 0.5f, 0.5f,-0.5f,		1.0f,1.0f,
	 0.5f, 0.5f, 0.5f,		1.0f,0.0f,
	 0.5f, 0.5f, 0.5f,		1.0f,0.0f,
	-0.5f, 0.5f, 0.5f,		0.0f,0.0f,
	-0.5f, 0.5f,-0.5f,		0.0f,1.0f
};

glm::vec3 cubePositions[] = {
 glm::vec3(0.0f, 0.0f, 0.0f),
 glm::vec3(2.0f, 5.0f,-15.0f),
 glm::vec3(-1.5f,-2.2f,-2.5f),
 glm::vec3(-3.8f,-2.0f,-12.3f),
 glm::vec3(2.4f,-0.4f,-3.5f),
 glm::vec3(-1.7f, 3.0f,-7.5f),
 glm::vec3(1.3f,-2.0f,-2.5f),
 glm::vec3(1.5f, 2.0f,-2.5f),
 glm::vec3(1.5f, 0.2f,-1.5f),
 glm::vec3(-1.3f, 1.0f,-1.5f)
};
#pragma endregion

int main() {
	float farPlane = 200.0f;
	float nearPlane = 0.1f;
	int width = 800, height = 600;
	vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	mat4 trans = mat4(1.0f);
	//trans = translate(trans, vec3(1.0f, 1.0f, 0.0f));
	//trans = rotate(trans, radians(90.0f), vec3(0.0, 0.0, 1.0));
	//trans = scale(trans, vec3(0.5, 0.5, 0.5));




	mat4 model = mat4(1.0f);
	model = rotate(model, radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));
	mat4 view = mat4(1.0f);
	view = translate(view, vec3(0.0f, 0.0f, -3.0f));


	mat4 orthoMat = ortho(0.0f, 800.0f, 0.0f, 600.0f, nearPlane, farPlane);
	mat4 projeMat = perspective(radians(45.0f), (float)width / (float)height, nearPlane, farPlane);
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
	GLFWwindow* window = glfwCreateWindow(width, height, "Game Engine", NULL, NULL);
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
	glEnable(GL_DEPTH_TEST);

	// Vertex Buffer Object id
	unsigned int vertexShaderId;
	unsigned int fragShaderId;

	Shader textureAndVertexColor = Shader("./Assets/Shaders/TexturedColorfulVertexShaderOffsetPos.vs", "./Assets/Shaders/TexturedInputVertexColor.fs");
	unsigned int VBO = 0;
	unsigned int VAO = 0;

	GameObject* gameObjects[10]{ nullptr };

	// This is not optimal, as all 10 objects are using the same vertex array
	// some optimization could be done, more on that in the future.
	for (int i = 0; i < 10; i++) {
		TransformComponent* transformComponent = new TransformComponent();
		transformComponent->setPos(cubePositions[i]);
		float angle = 20.0f * i;
		transformComponent->setRot(vec3(20.0f * i, 20.0f * i * 0.3, 20.0f * i * 0.5));
		MeshRenderer* meshRenderer = new VAOMeshRenderer(&cubeVertices[0], sizeof(cubeVertices) / sizeof(float), 36);
		Renderer3D* renderer3DComponent = new Renderer3D(&textureAndVertexColor, meshRenderer, transformComponent);
		gameObjects[i] = new GameObject(transformComponent, renderer3DComponent);
		// Cube Vertex Attribs
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	unsigned int brickWallTexId = LoadImages("./Assets/Images/brick_wall.jpg", GL_RGB);
	unsigned int emojiTexId = LoadImages("./Assets/Images/emoji.png", GL_RGBA);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickWallTexId);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, emojiTexId);

	unsigned int transformLoc = glGetUniformLocation(textureAndVertexColor.programId, "transform");
	unsigned int modelLoc = glGetUniformLocation(textureAndVertexColor.programId, "model");
	unsigned int viewLoc = glGetUniformLocation(textureAndVertexColor.programId, "view");
	unsigned int projectionLoc = glGetUniformLocation(textureAndVertexColor.programId, "projection");

	float offsetX = 0, offsetY = 0, mixRate = 0, numberOfFaces = 1;
	bool threadClosed = false;
	thread newThread(inputLoop, configs, &threadClosed);
	while (!glfwWindowShouldClose(window) && !*configs->closeApp) {
		model = rotate(model, (float)glfwGetTime() * radians(1.0f), vec3(0.5f, 1.0f, 0.0f));
		// processing inputs
		processInput(window);

		offsetX = 0;
		offsetY = 0;
		// Small WASD command to play around with the mesh, will not look like this in the end
		if (glfwGetKey(window, GLFW_KEY_W))
			offsetY = 0.05;
		else if (glfwGetKey(window, GLFW_KEY_S))
			offsetY = - 0.05;

		if (glfwGetKey(window, GLFW_KEY_A))
			offsetX = 0.05;
		else if (glfwGetKey(window, GLFW_KEY_D))
			offsetX = -0.05;
		
		view = translate(view, vec3(offsetX,0.0f, offsetY));

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//textureAndVertexColor.Use();
		textureAndVertexColor.SetInt("ourTexture", 0);
		textureAndVertexColor.SetInt("ourSecondTexture", 1);
		textureAndVertexColor.SetFloat("mixRate", mixRate);
		textureAndVertexColor.SetFloat("numberOfFaces", numberOfFaces);
		textureAndVertexColor.SetVector("offsetPos", offsetX, offsetY);
		for (int i = 0; i < 10; i++) {
			gameObjects[i]->_renderer3D->Update(&view, &projeMat, configs->renderWireframe);
		}

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

void renderTriangle(unsigned int* vbo, unsigned int vertexCount) {
	glBindVertexArray(*vbo);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void setupHelloTriangle(unsigned int* vbo, unsigned int* vao) {
	glGenBuffers(1, vbo);
	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

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

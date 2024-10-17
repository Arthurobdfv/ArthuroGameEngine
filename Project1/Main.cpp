
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
#include "Components/Camera/Camera.h"
#include "Components/Light/BaseLight.h"
#include <sstream>

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


int width = 1600, height = 900;
float lastX = width / 2, lastY = height / 2;
float yawAngle = -90.0f, pitchAngle = 0.0f;

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
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
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

float cubeEBO[] = {
	-0.25f,-0.25f,-0.25f, // Back	Bottom	Left	0
	-0.25f,-0.25f, 0.25f, // Front Bottom	Left	1
	-0.25f, 0.25f,-0.25f, // Back	Top		Left	2
	-0.25f, 0.25f, 0.25f, // Front Top		Left	3
	 0.25f,-0.25f,-0.25f, // Back	Bottom	Right	4
	 0.25f,-0.25f, 0.25f, // Front Bottom  Right	5
	 0.25f, 0.25f,-0.25f, // Back	Top		Right	6
	 0.25f, 0.25f, 0.25f, // Front Top		Right	7
};

unsigned int cubeEBOIndexes[] = {
	0,1,2,	// Left		Face
	2,1,3,	// Left		Face
	4,0,6,	// Front	Face
	6,0,2,	// Front	Face
	4,5,1,	// Top		Face
	4,1,0,	// Top		Face
	4,5,6,	// Back		Face
	6,5,7,	// Back		Face
	5,1,7,	// Rigkt	Face
	7,1,3,	// Right	Face
	7,3,6,	// Bottom	Face
	6,3,2	// Bottom	Face
};

float cubeVertices[] = {
	-0.5f,-0.5f,-0.5f,		0.0f, 0.0f,		0.0f, 0.0f,-1.0f,
	 0.5f,-0.5f,-0.5f,		1.0f, 0.0f,		0.0f, 0.0f,-1.0f,
	 0.5f, 0.5f,-0.5f,		1.0f, 1.0f,		0.0f, 0.0f,-1.0f,
	 0.5f, 0.5f,-0.5f,		1.0f, 1.0f,		0.0f, 0.0f,-1.0f,
	-0.5f, 0.5f,-0.5f,		0.0f,1.0f,		0.0f, 0.0f,-1.0f,
	-0.5f,-0.5f,-0.5f,		0.0f,0.0f,		0.0f, 0.0f,-1.0f,

	-0.5f,-0.5f, 0.5f,		0.0f,0.0f,		0.0f, 0.0f, 1.0f,
	0.5f,-0.5f, 0.5f,		1.0f,0.0f,		0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f,		1.0f,1.0f,		0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f,		1.0f,1.0f,		0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,		0.0f,1.0f,		0.0f, 0.0f, 1.0f,
	-0.5f,-0.5f, 0.5f,		0.0f,0.0f,		0.0f, 0.0f, 1.0f

	,-0.5f, 0.5f, 0.5f,		1.0f,0.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f,-0.5f,		1.0f,1.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f,-0.5f,-0.5f,		0.0f,1.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f,-0.5f,-0.5f,		0.0f,1.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f,-0.5f, 0.5f,		0.0f,0.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		1.0f,0.0f,		-1.0f, 0.0f, 0.0f,

	 0.5f, 0.5f, 0.5f,		1.0f,0.0f,		1.0f, 0.0f, 0.0f,
	 0.5f, 0.5f,-0.5f,		1.0f,1.0f,		1.0f, 0.0f, 0.0f,
	 0.5f,-0.5f,-0.5f,		0.0f,1.0f,		1.0f, 0.0f, 0.0f,
	 0.5f,-0.5f,-0.5f,		0.0f,1.0f,		1.0f, 0.0f, 0.0f,
	 0.5f,-0.5f, 0.5f,		0.0f,0.0f,		1.0f, 0.0f, 0.0f,
	 0.5f, 0.5f, 0.5f,		1.0f,0.0f,		1.0f, 0.0f, 0.0f,

	-0.5f,-0.5f,-0.5f,		0.0f,1.0f,		0.0f,-1.0f, 0.0f,
	 0.5f,-0.5f,-0.5f,		1.0f,1.0f,		0.0f,-1.0f, 0.0f,
	 0.5f,-0.5f, 0.5f,		1.0f,0.0f,		0.0f,-1.0f, 0.0f,
	 0.5f,-0.5f, 0.5f,		1.0f,0.0f,		0.0f,-1.0f, 0.0f,
	-0.5f,-0.5f, 0.5f,		0.0f,0.0f,		0.0f,-1.0f, 0.0f,
	-0.5f,-0.5f,-0.5f,		0.0f,1.0f,		0.0f,-1.0f, 0.0f,

	-0.5f, 0.5f,-0.5f,		0.0f,1.0f,		0.0f, 1.0f, 0.0f,
	 0.5f, 0.5f,-0.5f,		1.0f,1.0f,		0.0f, 1.0f, 0.0f,
	 0.5f, 0.5f, 0.5f,		1.0f,0.0f,		0.0f, 1.0f, 0.0f,
	 0.5f, 0.5f, 0.5f,		1.0f,0.0f,		0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		0.0f,0.0f,		0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f,-0.5f,		0.0f,1.0f,		0.0f, 1.0f, 0.0f
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

	vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);


	float cameraSpeed = 3.0f;

	mat4 model = mat4(1.0f);
	model = rotate(model, radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));
	mat4 view = mat4(1.0f);

	Camera* camera = new Camera(vec3(0.0f, 0.0f, 3.0f));
	vec3 cameraTarget = vec3(0.0f);
	vec3 cameraDirection = normalize(camera->Position() - cameraTarget);
	vec3 worldUp = vec3(0.0f, 1.0f, 0.0f);
	vec3 cameraRight = normalize(cross(worldUp, cameraDirection));
	vec3 cameraUp = cross(cameraDirection, cameraRight);
	view = lookAt(
		vec3(0.0f, 0.0f, 3.0f),
		vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f)
	);
	vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

	mat4 orthoMat = ortho(0.0f, 800.0f, 0.0f, 600.0f, nearPlane, farPlane);
	mat4 projeMat = perspective(radians(45.0f), (float)width / (float)height, nearPlane, farPlane);

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
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	bool currentCursorEnabled = false;
	float cursorChangeDelay = 0.0f;
	
	
	glfwSetCursorPosCallback(window, mouseCallback);
	// Vertex Buffer Object id
	unsigned int vertexShaderId;
	unsigned int fragShaderId;

	Shader litTextureAndVertexColor = Shader("./Assets/Shaders/LitTexturedColorfulVertexShaderOffsetPos.vs", "./Assets/Shaders/LitTexturedInputVertexColor.fs");
	Shader singleColored = Shader("./Assets/Shaders/SingleColored3D/SimpleColored.vs", "./Assets/Shaders/SingleColored3D/SimpleColored.fs");
	unsigned int VBO = 0;
	unsigned int VAO = 0;
	

	vec3 pointLightColor = vec3(0, 0.5, 0.5);
	float pointLightIntensity = 0.5f;
	vec3 pointLightPosition = vec3(2, 0, 2);

	// Point Light Creation;
	float* cubeEBOAddr = &cubeEBO[0];
	MeshRenderer* lightMeshRenderer = new EBOMeshRenderer(cubeEBOAddr, sizeof(cubeEBO) / sizeof(float), &cubeEBOIndexes[0], sizeof(cubeEBOIndexes) / sizeof(int));
	TransformComponent* lightTransform = new TransformComponent();
	lightTransform->setPos(pointLightPosition);
	GameObject simpleLightObject = GameObject(lightTransform, new Renderer3D(&singleColored, lightMeshRenderer, lightTransform));
	singleColored.Use();
	singleColored.SetVector4("aColor", vec4(pointLightColor, pointLightIntensity));
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	BaseLight* pointLight = new BaseLight(pointLightColor, pointLightPosition, Point, pointLightIntensity);
	// Hardcoded for now, will change in near future.
	const int lightDataSize = 8;
	const unsigned int lightCount = 3;
	BaseLight* LightData[lightCount] = {
		new BaseLight(vec3(.2f,.2f,1.0f), vec3(0,0,0), Ambient, 0.2f),
		new BaseLight(vec3(1.0f, .2f, .2f), vec3(0,.5,1), Directional, 0.8f),
		pointLight
	};


	GameObject* gameObjects[10]{ nullptr };
	Shader shaderToUse = litTextureAndVertexColor;
	shaderToUse.Use();
	shaderToUse.SetInt("lightCount", lightCount);
	for (int i = 0; i < lightCount; i++) {
		stringstream ss;
		ss << "lightData[" << i << "].";
		vec4 color = vec4(LightData[i]->Color(),LightData[i]->Intensity());
		shaderToUse.SetVector3("cameraPos", camera->Position());
		shaderToUse.SetVector(ss.str() + "color", color.x, color.y, color.z, color.w);
		shaderToUse.SetVector3(ss.str() + "orientation", LightData[i]->Orientation());
		shaderToUse.SetInt(ss.str() + "lightType", LightData[i]->Type());
		shaderToUse.SetFloat(ss.str() + "intensity", LightData[i]->Intensity());
	}
	// This is not optimal, as all 10 objects are using the same vertex array
	// some optimization could be done, more on that in the future.
	
	int cubesToRender = 10;
	for (int i = 0; i < cubesToRender; i++) {
		TransformComponent* transformComponent = new TransformComponent();
		transformComponent->setPos(cubePositions[i]);
		float angle = 20.0f * i;
		transformComponent->setRot(vec3(20.0f * i, 20.0f * i * 0.3, 20.0f * i * 0.5));
		MeshRenderer* meshRenderer = new VAOMeshRenderer(&cubeVertices[0], sizeof(cubeVertices) / sizeof(float), 36);
		Renderer3D* renderer3DComponent = new Renderer3D(&shaderToUse, meshRenderer, transformComponent);
		gameObjects[i] = new GameObject(transformComponent, renderer3DComponent);
		// Cube Vertex Attribs
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5*sizeof(float)));
		glEnableVertexAttribArray(3);
	}

	unsigned int brickWallTexId = LoadImages("./Assets/Images/brick_wall.jpg", GL_RGB);
	unsigned int emojiTexId = LoadImages("./Assets/Images/emoji.png", GL_RGBA);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickWallTexId);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, emojiTexId);


	float offsetX = 0, offsetY = 0, mixRate = 0, numberOfFaces = 1;
	bool threadClosed = false;
	thread newThread(inputLoop, configs, &threadClosed);
	float deltaTime = 0.0f, lastFrame = 0.0f;
	while (!glfwWindowShouldClose(window) && !*configs->closeApp) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		model = rotate(model, (float)glfwGetTime() * radians(1.0f), vec3(0.5f, 1.0f, 0.0f));
		// processing inputs
		processInput(window);

		offsetX = 0;
		offsetY = 0;
		vec3 movement = vec3(0.0f);
		// Small WASD command to play around with the mesh, will not look like this in the end
		if (glfwGetKey(window, GLFW_KEY_W))
			movement += cameraSpeed * cameraFront;
		else if (glfwGetKey(window, GLFW_KEY_S))
			movement -= cameraSpeed * cameraFront;

		if (glfwGetKey(window, GLFW_KEY_A))
			movement -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
		else if (glfwGetKey(window, GLFW_KEY_D))
			movement += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;

		// Arrow key movement to play around with point light
		vec3 pointLightMovement = vec3(0, 0, 0);
		if (glfwGetKey(window, GLFW_KEY_UP))
			pointLightMovement -= vec3(0,0,cameraSpeed * deltaTime);
		else if (glfwGetKey(window, GLFW_KEY_DOWN))
			pointLightMovement += vec3(0,0,cameraSpeed * deltaTime);

		if (glfwGetKey(window, GLFW_KEY_RIGHT))
			pointLightMovement += vec3(cameraSpeed * deltaTime, 0, 0);
		else if (glfwGetKey(window, GLFW_KEY_LEFT))
			pointLightMovement -= vec3(cameraSpeed * deltaTime, 0, 0);

		simpleLightObject.Transform->setPos(simpleLightObject.Transform->Position() + pointLightMovement);
		pointLight->SetOrientation(simpleLightObject.Transform->Position());

		vec3 direction;
		direction.x = cos(radians(yawAngle)) * cos(radians(pitchAngle));
		direction.y = sin(radians(pitchAngle));
		direction.z = sin(radians(yawAngle)) * cos(radians(pitchAngle));
		camera->SetPosition(camera->Position() + movement * deltaTime);

		if (glfwGetKey(window, GLFW_KEY_F) && cursorChangeDelay > 0.5f) {
			currentCursorEnabled = !currentCursorEnabled;
			if(currentCursorEnabled)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			cursorChangeDelay = 0.0f;
		}
		cursorChangeDelay += deltaTime;
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

		const float radius = 10.0f;

		cameraFront = normalize(direction);
		view = glm::lookAt(camera->Position(), camera->Position() + cameraFront, cameraUp);
		
		//view = translate(view, vec3(offsetX,0.0f, offsetY));
		// rendernig commands
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//textureAndVertexColor.Use();

		// Need to optimize this
		for (int i = 0; i < lightCount; i++) {
			stringstream ss;
			ss << "lightData[" << i << "].";
			vec4 color = vec4(LightData[i]->Color(), LightData[i]->Intensity());
			shaderToUse.SetVector(ss.str() + "color", color.x, color.y, color.z, color.w);
			shaderToUse.SetVector3("cameraPos", camera->Position());
			shaderToUse.SetVector3(ss.str() + "orientation", LightData[i]->Orientation());
			shaderToUse.SetInt(ss.str() + "lightType", LightData[i]->Type());
			shaderToUse.SetFloat(ss.str() + "intensity", LightData[i]->Intensity());
		}
		
		simpleLightObject._renderer3D->Update(&view, &projeMat, configs->renderWireframe);
		for (int i = 0; i < cubesToRender; i++) {
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

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yawAngle += xoffset;
	pitchAngle += yoffset;
	if (pitchAngle > 89.0f)
		pitchAngle = 89.0f;
	if (pitchAngle < -89.0f)
		pitchAngle = -89.0f;
};
#pragma endregion

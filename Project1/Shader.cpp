#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "Constants/EngineConstants.cpp"
using namespace glm;
using namespace EngineConstants;

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	string vertexCode, fragmentCode;
	ifstream vertexShaderFile;
	ifstream fragmentShaderFile;
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		//openfiles
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);
		stringstream vShaderStream, fShaderStream;
		//readfile’sbuffercontentsintostreams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		//closefilehandlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		//convertstreamintostring
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch (ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ " << e.what() << std::endl;
	}
	const char* vertexShaderCode = vertexCode.c_str();
	const char* fragmentShaderCode = fragmentCode.c_str();
	//2.compileshaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	//vertexShader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderCode, NULL);
	glCompileShader(vertex);
	//printcompileerrorsifany
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	};

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragment);
	//printcompileerrorsifany
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	};

	programId = glCreateProgram();
	glAttachShader(programId, vertex);
	glAttachShader(programId, fragment);
	glLinkProgram(programId);
	//printlinkingerrorsifany
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
			infoLog << std::endl;
	}
	//deleteshaders;they’relinkedintoourprogramandnolongernecessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	AssignMVPUniformLocations();
}

void Shader::Use()
{
	glUseProgram(programId);
}

void Shader::SetBool(const string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(programId, name.c_str()), (int)value);
}

void Shader::SetInt(const string& name, int value) const
{
	glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::SetFloat(const string& name, float value) const
{
	glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::SetVector(const string& name, float x, float y, float z, float w)
{
	unsigned int uniLoc = glGetUniformLocation(programId, name.c_str());
	glUniform4f(uniLoc, x, y, z, w);
}

void Shader::SetVector3(const string& name, float x, float y, float z) {
	unsigned int uniLoc = glGetUniformLocation(programId, name.c_str());
	glUniform3f(uniLoc, x, y, z);
}

void Shader::SetVector4(const string& name, vec4 vector) {
	SetVector(name, vector.x, vector.y, vector.z, vector.w);
}

void Shader::SetVector3(const string& name, vec3 vec3) {
	SetVector3(name, vec3.x, vec3.y, vec3.z);
}

void Shader::SetMat4(const string& name, mat4 matData)
{
	glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(matData));
}

void Shader::SetModelMatrix(mat4* matData)
{
	if (_modelMatrixLocation != 0)
		glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(*matData));
	else
		cout << "Model Matrix uniform matrix not found!" << endl;
}

void Shader::SetViewMatrix(mat4* matData)
{
	if (_viewMatrixLocation != 0)
		glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(*matData));
	else
		cout << "View Matrix uniform matrix not found!" << endl;
}

void Shader::SetProjectionMatrix(mat4* matData)
{
	if (_projectionMatrixLocation != 0)
		glUniformMatrix4fv(_projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(*matData));
	else
		cout << "Projection Matrix uniform matrix not found!" << endl;
}

int Shader::GetUniformLocation(const string& name)
{
	int uniformLocation = glGetUniformLocation(programId, name.c_str());
	if (uniformLocation < 0)
		cout << "Could not find Uniform named:" << name << " in shader Id:" << programId << endl;
	return uniformLocation;
}

void Shader::AssignMVPUniformLocations()
{
	_modelMatrixLocation = GetUniformLocation(SHADER_MODEL_MATRIX_NAME);
	_viewMatrixLocation = GetUniformLocation(SHADER_VIEW_MATRIX_NAME);
	_projectionMatrixLocation = GetUniformLocation(SHADER_PROJECTION_MATRIX_NAME);
}


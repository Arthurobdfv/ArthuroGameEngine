#pragma once
#include <string>
#include "glm/gtc/matrix_transform.hpp"

using namespace std;
using namespace glm;

class Shader {
	public:
		unsigned programId;

		Shader(const char* vertexPath, const char* fragmentPath);
		void Use();
		void SetBool(const string& name, bool value) const;
		void SetInt(const string& name, int value) const;
		void SetFloat(const string &name, float value) const;
		void SetVector(const string& name, float x = 0, float y = 0, float z = 0, float w = 0);
		void SetVector3(const string& name, float x, float y, float z);
		void SetVector4(const string& name, vec4 vector);
		void SetVector3(const string& name, vec3 vec3);
		void SetMat4(const string& name, mat4 matData);
		void SetModelMatrix(mat4* matData);
		void SetViewMatrix(mat4* matData);
		void SetProjectionMatrix(mat4* matData);

	private:
		int GetUniformLocation(const string& name);
		void AssignMVPUniformLocations();
		unsigned int _modelMatrixLocation = 0, _viewMatrixLocation = 0, _projectionMatrixLocation = 0;
};
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
		void SetMat4(const string& name, mat4 matData);
};
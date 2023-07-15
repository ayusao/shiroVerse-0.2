#pragma once

#ifndef SHADER_H
#define SHADER_H

#include<string>

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>

//Grneral purpose shader object. Compiles from file, generates compile/link-time error messages and hosts several utility
class Shader
{
private:
	void checkCompileErrors(unsigned int object, std::string type); //checks if compilation or linking failed and print the error
public:
	unsigned int ID; //state

	Shader() {}

	Shader& Use(); //set the current shader to active

	void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr); //compiles the source code from given source code

	void SetFloat		(const char* name, float value, bool useShader = false);
	void SetInteger		(const char* name, int value, bool useShader = false);
	void SetVector2f	(const char* name, float x, float y, bool useShader = false);
	void SetVector2f	(const char* name, const glm::vec2& value, bool useShader = false);
	void SetVector3f	(const char* name, float x, float y, float z, bool useShader = false);
	void SetVector3f	(const char* name, const glm::vec3& value, bool useShader );
	void SetVector4f	(const char* name, float x, float y, float z, float w, bool useShader);
	void SetVector4f	(const char* name, const glm::vec4& value, bool useShader );
	void SetMatrix4		(const char* name, const glm::mat4& matrix, bool useShader );
};

#endif

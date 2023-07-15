#pragma once

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include<map>
#include<string>

#include<glad/glad.h>

#include "texture.h"
#include "shader.h"

//A stactic singleton ResourceManager class that hosts several functions to load Textures and shaders
// 
//
class ResourceManager {
private:
	ResourceManager(){}

	static Shader	loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);

	static Texture2D loadTextureFromFile(const char* file, bool alpha);

public:
	//resource storage
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;

	//loads (generates) a shader program from file loading vertex,fragment
	static Shader	LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);

	//retrives a stored shader
	static Shader	GetShader(std::string name);

	//loads(generates) a texture from file
	static Texture2D LoadTexture(const char* file, bool alpha, std::string name);

	//retrives a stored texture
	static Texture2D GetTexture(std::string name);

	//properly de-allocates all loaded resources
	static void Clear();
};
#endif
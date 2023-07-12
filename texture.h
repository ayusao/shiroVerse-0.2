#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include<glad/glad.h>

//Texture2D is able to store and configure a texture in OpenGL.

class Texture2D {
public:
	//holds the ID of the texture object
	unsigned int ID;

	//texture image dimensions in pixel
	unsigned int Width, Height;

	//texture Format
	unsigned int InternalFormat;	//texture object
	unsigned int ImageFormat;		//format of loaded image

	//texture configuration
	unsigned int WrapS;			//wrapping mode on S axis
	unsigned int WrapT;			//wrapping mode on T axis
	unsigned int FilterMin;		//filtering mode if texture pixels < screen pixels
	unsigned int FilterMax;		//filtering mode if texture pixels > screen pixels  

	//constructor (sets default texture modes)
	Texture2D();

	//generates texture from image data
	void Generate(unsigned int width, unsigned int height, unsigned char* data);

	//binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;
};

#endif

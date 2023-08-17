#include<iostream>
#include "texture.h"

Texture2D::Texture2D()
	:Width(0),Height(0),InternalFormat(GL_RGB), ImageFormat(GL_RGB), WrapS(GL_REPEAT), WrapT(GL_REPEAT), FilterMin(GL_LINEAR), FilterMax(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data){
	this->Width = width;
	this->Height = height;

	//create texture
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->InternalFormat, width, height, 0, this->ImageFormat, GL_UNSIGNED_BYTE, data);

	//set texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->WrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->WrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->FilterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->FilterMax);

	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const {
	glBindTexture(GL_TEXTURE_2D, this->ID);
}
const char* Texture2D::GetName() const {
	return textureName;
}
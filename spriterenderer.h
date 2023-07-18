#pragma once
#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

class SpriteRenderer {
private:
	Shader		shader;
	unsigned int quadVAO;
	//initializes and configures the quad's buffer and vertex attributes
	void initRenderData();
public:
	SpriteRenderer(Shader &shader);
	~SpriteRenderer();

	//renders a defined quad textured with given sprite
	void DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
};

#endif
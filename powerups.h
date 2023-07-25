#pragma once
#ifndef POWERUPS_H
#define POWERIPS_H

#include <glad/glad.h>
#include<glm/glm.hpp>
#include "gameobject.h"

const glm::vec2 POWERUP_SIZE(60.0F, 20.0F);
const glm::vec2 VELOCITY(0.0f, 150.0F);

class PowerUp : public GameObject {
public:
	std::string Type;
	float Duration;
	bool Activated;

	//constructor
	PowerUp(std::string type, glm::vec3 color,float duration, glm::vec2 position, Texture2D texture)
		:GameObject(position, POWERUP_SIZE, texture, color, VELOCITY),
		Type(type),Duration(duration),Activated(){}
};
#endif
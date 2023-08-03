#pragma once
#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H

#include<glad/glad.h>
#include<glm/glm.hpp>

#include "gameobject.h"
#include "texture.h"

//BallObject holds the state of the Ball object inheriting relevent state data from gameObject

class PlayerObject : public GameObject {
public:
	float Radius;
	bool Stuck = false;
	bool Sticky, PassThrough;
	bool		Keys[1024];

	PlayerObject();
	PlayerObject(glm::vec2 position, float radius, glm::vec2 velocity, Texture2D sprite);

	//to limit the ball inside the window bounds
	glm::vec2 Move(float dt, unsigned int window_width);

	//reset the ball to original state with given position and velocity
	void Reset(glm::vec2 position, glm::vec2 velocity);
};
#endif
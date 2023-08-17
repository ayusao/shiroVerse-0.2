#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include<glad/glad.h>
#include<glm/glm.hpp>

#include "texture.h"
#include "spriterenderer.h"

//container object for holding all state relevant for a single game object entity

class GameObject {
public:
	//object state
	glm::vec2 Position, Size, Velocity;
	glm::vec3 Color;
	float	   Rotation;
	bool	  IsSolid;
	bool	  Destroyed;

	/*float Radius;	
	bool Stuck = false;
	bool PassThrough, Sticky;
	bool Keys[1024];*/

	//render state
	Texture2D Sprite;

	//constructor(s)
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	//draw sprite
	virtual void Draw(SpriteRenderer& renderer);
};
#endif 

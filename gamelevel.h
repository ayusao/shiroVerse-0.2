#pragma once

#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include<glad/glad.h>
#include<glm/glm.hpp>

#include "gameobject.h"
#include "spriterenderer.h"
#include "resourceManager.h"

//hosts functionality to load/render levels from the harddisk.

class GameLevel {

public:
	//level state
	std::vector<GameObject> Bricks;
	//constructor
	GameLevel() {}
	//loads level from file
	void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
	//render level
	void Draw(SpriteRenderer& renderer);
	//check if the level is completed
	bool IsCompleted(); 
private:
		//initialize level 
		void init(std::vector < std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);


};
#endif

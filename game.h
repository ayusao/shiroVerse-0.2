#pragma once

#ifndef GAME_H
#define GAME_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "gamelevel.h"

//represents the current state of the game
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);

const float PLAYER_VELOCITY(500.0f);

//ease access to each of the components and manafeability
class Game {
public:
	//game state
	GameState State;
	bool		Keys[1024];
	unsigned int	Width, Height;
	std::vector<GameLevel>	Levels;
	unsigned int			Level;
	//constructor/deconstructor
	Game(unsigned int width, unsigned int height);
	~Game();

	//initialize game state (load all shaders/textures/levels)
	void Init();

	//game loop
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();
};

#endif
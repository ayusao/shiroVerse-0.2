#pragma once

#ifndef GAME_H
#define GAME_H
#include<vector>
#include<tuple>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "gamelevel.h"
#include "powerups.h"

//represents the current state of the game
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
	GAME_OVER,
	HELP_MENU
};

//represents the four possible (collision) directions
enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

//a collision typedef that represents collision data
typedef std::tuple<bool, Direction, glm::vec2> Collision; //<collision?, what direction?, difference vector center - closest point>

//the player paddle
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);

//paddle initial velocity
const float PLAYER_VELOCITY(200.0f);

//radius of the ball object
const float BALL_RADIUS = 18.0f;

//initial ball velocity
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -250.0f);

//shark
const float sharkRadius = 26.0f;
const glm::vec2 sharkVelocityleft(-160.0f, 0.0f);  // Move left
const glm::vec2 sharkVelocityright(160.0f, 0.0f);   // Move right

//easy access to each of the components and manafeability
class Game {
public:
	//game state
	GameState State;
	bool		Keys[1024];
	bool		KeysProcessed[1024];
	unsigned int	Width, Height;
	std::vector<GameLevel>	Levels;
	unsigned int			Level;
	unsigned int			Lives;
	unsigned int			levelSelect;
	std::vector<PowerUp> PowerUps;
	//constructor/deconstructor
	Game(unsigned int width, unsigned int height);
	~Game();

	//initialize game state (load all shaders/textures/levels)
	void Init();

	//game loop
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();
	void DoCollisions();

	//reset
	void ResetLevel();
	void ResetPlayer();

	//power ups
	void SpawnPowerUps(GameObject& block);
	void UpdatePowerUps(float dt);
};

#endif
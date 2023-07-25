#pragma once
#ifndef PARTICLEGENERATOR_H
#define PARTICLEGENERATOR_H

#include<vector>

#include<glad/glad.h>
#include<glm/glm.hpp>

#include"shader.h"
#include "texture.h"
#include "gameobject.h"

//representing a single particle and its state
struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	float Life;

	Particle() :Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {};
};

//The class acts as a container for rendering a large number of  particles by repeatedly
//spawning and updating particles and killing them after given amount of time

class ParticleGenerator {
private:
	//state
	std::vector<Particle> particles;
	unsigned int amount;
	//render state
	Shader shader;
	Texture2D texture;
	unsigned int VAO;

	//initialize buffer and vertex attributes
	void init();
	//return the first particle index that's currently unused i.e. life <= 0.0f or 0 if no particle is currently inactive
	unsigned int firstUnusedParticle();
	//respawns particle
	void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
public:
	ParticleGenerator(Shader shader, Texture2D texture,unsigned int amount);
	void Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	void Draw();
};

#endif 

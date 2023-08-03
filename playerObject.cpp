#include "playerObject.h"

PlayerObject::PlayerObject()
	:GameObject(),Radius(12.5f),Stuck(true),Sticky(false),PassThrough(false){}

PlayerObject::PlayerObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
	: GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(true), Sticky(false), PassThrough(false) { }

glm::vec2 PlayerObject::Move(float dt, unsigned int window_width) {
	//if not struck to playerboard
	if (!this->Stuck) {
		//move


		//this->Position += this->Velocity * dt;
		////check if outside window bounds and if so, reverse velocity and restore at correct position
		//if (this->Position.x <= 0.0f) {
		//	this->Velocity.x = -this->Velocity.x;
		//	this->Position.x = 0.0f;
		//}
		//else if (this->Position.x + this->Size.x >= window_width) {
		//	this->Velocity.x = -this->Velocity.x;
		//	this->Position.x = window_width - this->Size.x;
		//}
		//if (this->Position.y <= 0.0f) {
		//	this->Velocity.y = -this->Velocity.y;
		//	this->Position.y = 0.0f;
		//}
	}
	return this->Position;
}

//reset the ball to initial Stuck Position(outside the window bound)
void PlayerObject::Reset(glm::vec2 position, glm::vec2 velocity) {
	this->Position = position;
	this->Velocity = velocity;
	this->Stuck = false;
	this->Sticky = false;
	this->PassThrough = false;
}
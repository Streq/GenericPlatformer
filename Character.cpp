/*
 * Character.cpp
 *
 *  Created on: Sep 14, 2017
 *      Author: santiago
 */

#include "Character.hpp"
void Character::moveLeft() {
	state.vecAcceleration.x -= properties.acceleration;
}

void Character::moveRight() {
	state.vecAcceleration.x += properties.acceleration;
}

void Character::moveUp() {
	state.vecAcceleration.y -= properties.acceleration;
}

void Character::moveDown() {
	state.vecAcceleration.y += properties.acceleration;
}

void Character::jump() {
	if(grounded)
		state.velocity.y-=properties.jumpSpeed;
}

Character::Character(
		const Properties& properties,
		const State& state)
:
		properties(properties),
		state(state)
{
	bot_wall=left_wall=right_wall=top_wall=grounded=false;
}

Character::Character(
		const Properties& properties)
:
		properties(properties)
{

	bot_wall=left_wall=right_wall=top_wall=grounded=false;
}

void Character::update(float dt) {
	state.vecAcceleration += properties.gravity;
	state.velocity += state.vecAcceleration*dt;

	state.position += state.velocity;

	auto norm = mch::vec::normalized(state.velocity);
	state.velocity.x = mch::approach<float>(state.velocity.x, 0.f, copysign(norm.x,1.f)*properties.friction*dt);
	state.velocity.y = mch::approach<float>(state.velocity.y, 0.f, copysign(norm.y,1.f)*properties.friction*dt);
	state.velocity *= (1.f-properties.buoyancy*dt);
	state.vecAcceleration.x=0.f;
	state.vecAcceleration.y=0.f;


}

Character::Character() {
	bot_wall=left_wall=right_wall=top_wall=grounded=false;
}

void Character::setProperties(const Properties& properties) {
	this->properties = properties;
}

void Character::setState(const State& state) {
	this->state = state;
}

mch::Vec2f Character::getPosition() {
	return state.position;
}

mch::AABB Character::getBoundaries() {
	mch::AABB ret;

	mch::Vec2f center {properties.radius, properties.radius};
	mch::Vec2f position = state.position - center;

	ret.size = center*2.f;
	ret.topleft = position-center;
	return ret;
}

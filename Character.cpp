/*
 * Character.cpp
 *
 *  Created on: Sep 14, 2017
 *      Author: santiago
 */

#include "Character.hpp"

void Character::moveLeft() {
	vecAcceleration.x-=acceleration;
}

void Character::moveRight() {
	vecAcceleration.x+=acceleration;
}

void Character::moveUp() {

}

void Character::moveDown() {

}

void Character::jump() {
	if(grounded)
		velocity.y-=jumpSpeed;
}

void Character::update(float dt) {
	vecAcceleration += gravity;
	velocity += vecAcceleration*dt;
	position += velocity;
}

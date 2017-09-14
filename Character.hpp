/*
 * Character.hpp
 *
 *  Created on: Sep 14, 2017
 *      Author: santiago
 */

#pragma once

#include "definitions.hpp"
#include "vec2.hpp"

class Character {
	public:
		void moveLeft();
		void moveRight();
		void moveUp();
		void moveDown();
		void jump();

		void setGrounded(bool);
		void update(float dt);
	private:

		mch::Vec2					position;
		mch::Vec2 					vecAcceleration;
		mch::Vec2 					velocity;
		mch::Vec2 					gravity;
		mch::float32 				radius ;
		mch::float32 				acceleration;
		mch::float32 				buoyancy;
		mch::float32 				friction;
		mch::float32 				rebound;
		mch::float32				jumpSpeed;
		bool						grounded;
		bool						left_wall;
		bool						right_wall;
		bool						top_wall;
		bool						bot_wall;


};


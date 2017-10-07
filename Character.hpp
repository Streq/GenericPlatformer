/*
 * Character.hpp
 *
 *  Created on: Sep 14, 2017
 *      Author: santiago
 */

#pragma once

#include <Mocho/definitions.hpp>
#include <Mocho/vec2.hpp>

#include <Mocho/collision.hpp>

class Character {
	public:
		struct Properties{
			mch::Vec2f					gravity;
			mch::float32				radius ;
			mch::float32				acceleration;
			mch::float32				buoyancy;
			mch::float32				friction;
			mch::float32				rebound;
			mch::float32				jumpSpeed;
		};

		struct State{
			mch::Vec2f					position;
			mch::Vec2f					vecAcceleration;
			mch::Vec2f					velocity;
		};

	public:
		Character(const Properties& properties, const State& state);
		Character(const Properties& properties);
		Character();

		void setProperties(const Properties& properties);
		void setState(const State& state);

		void moveLeft();
		void moveRight();
		void moveUp();
		void moveDown();
		void jump();
		void shoot();

		void setGrounded(bool);
		void update(float dt);

		mch::Vec2f getPosition();
		mch::AABB getBoundaries();

	private:
		Properties					properties;
		State						state;
		bool						grounded;
		bool						left_wall;
		bool						right_wall;
		bool						top_wall;
		bool						bot_wall;

};


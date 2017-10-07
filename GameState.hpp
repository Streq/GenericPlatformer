/*
 * GameState.hpp
 *
 *  Created on: Sep 30, 2017
 *      Author: santiago
 */

#pragma once

#include <Mocho/Application/AppState.hpp>
#include <Mocho/definitions.hpp>
#include <Mocho/vec2.hpp>
#include <Mocho/collision.hpp>


#include <vector>

namespace mch {

class GameState: public AppState {
	public:

		//Update the state
		//@returns bool stating wether or not to keep updating
		// for next states in the stack
		virtual bool update();

		//Draw the current state
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;

		//Handle a given event
		//@returns bool stating wether or not to keep handling input
		// for next states in the stack
		virtual bool input(const sf::Event& event);

		void init();
	public:
		struct Character{
			Vec2f position;
			AABB  BoundingBox;
			Vec2f velocity;
			float speed;

			//get aabb in global coords
			AABB getGlobalAABB(){
				auto tempPjAABB = BoundingBox;
				tempPjAABB.topleft += position;
				return tempPjAABB;
			}
		};
		struct Platform{
			AABB  BoundingBox;
		};
		struct Input{
			enum action{
				up,
				down,
				left,
				right,
				size
			};
			bool map[size];
		};

		struct Collision{
			uint32 id1;
			//uint32 id2;
		};

	private:
		std::vector<Platform> m_platforms;
		Character m_pj;
		Input m_input;


		std::vector<Collision> m_collisions;


		void moveEntities(float dt);
};

} /* namespace mch */

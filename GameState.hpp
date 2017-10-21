/*
 * GameState.hpp
 *
 *  Created on: Sep 30, 2017
 *      Author: santiago
 */

#pragma once

#include <Mocho/Application/AppState.hpp>
#include <Mocho/Collision/collision.hpp>
#include <Mocho/definitions.hpp>
#include <Mocho/vec2.hpp>
#include <vector>

#include "Entities/Entity.hpp"
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
	private:
		float m_player_speed;
		float m_player_acceleration;
		std::vector<Entity> m_walls;
		std::vector<Player> m_characters;
		enum input_key{
			up,left,down,right,size
		};
		bool m_input[input_key::size];

	private:
		Vec2f getInputDirection();

};

} /* namespace mch */

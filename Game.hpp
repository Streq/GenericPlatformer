/*
 * Game.hpp
 *
 *  Created on: Sep 11, 2017
 *      Author: santiago
 */

#pragma once
#include <Mocho/definitions.hpp>
#include <Mocho/vec2.hpp>
#include <Mocho/window.hpp>
#include <Mocho/time.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Character.hpp"
namespace mch{

class Game {
	public:
		void run();

	private:
		void init();
		void update();
		void render();
		void handle_events();

		void setFPS(int fps);

	private:
		RenderWindow 					window;
		Vec2u 						windowSize;

		float32 					physicsDeltaSecs;
		Time 						physicsDeltaTime;;

		bool 						frameskip;
		bool 						limitFPS;
		uint64						msSinceLastRender;
		int32						stepsPerRender;

		sf::Font					fpsFont;
		sf::Text					fpsText;

		sf::CircleShape 			player;

		Character::Properties 		charProperties;
		Character::State			charInitState;

		Character 					character;

		bool 						up,down,left,right,jump;
};

}

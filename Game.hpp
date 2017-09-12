/*
 * Game.hpp
 *
 *  Created on: Sep 11, 2017
 *      Author: santiago
 */

#pragma once
#include "definitions.hpp"
#include "vec2.hpp"

#include "window.hpp"
#include "time.hpp"
#include <SFML/Graphics/CircleShape.hpp>

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
		RenderWindow 				window;
		Vec2u 						windowSize;



		uint32						fps;
		float32 					secondsPerFrame;
		Time 						timePerFrame;
		bool 						frameskip;
		bool 						limitFPS;

		sf::CircleShape 			player;

		Vec2 						vecSpeed{0.f,0.f};
		Vec2 						velocity{0.f,0.f};
		float32 					radius = 10.f;
		float32 					speed = 15.f;
		float32 					buoyancy = 0.05f;
		float32 					friction = 0.5f;
		float32 					rebound = 1.f;
		Vec2 						gravity{0.f,5.f};
		bool 						up,down,left,right;



};

}
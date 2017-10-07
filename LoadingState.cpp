/*
 * LoadingState.cpp
 *
 *  Created on: Sep 30, 2017
 *      Author: santiago
 */

#include "LoadingState.hpp"
#include "GameState.hpp"
#include <Mocho/Application/AppContext.hpp>

#include <SFML/System/Time.hpp>
namespace mch {


bool LoadingState::update(){
	auto& cont = this->getContext();
	auto& s=cont.settings;
	s.limit_framerate=true;
	s.frameskip=true;
	s.microseconds_per_step=sf::seconds(1.f/60.f).asMicroseconds();
	s.resolution={800,600};

	auto gameStateFactory = [](){
		auto* gameState = new GameState;
		gameState->init();
		return AppState::ptr(gameState);
	};

	requestPop();
	requestPush(gameStateFactory());




	return true;
}




} /* namespace mch */

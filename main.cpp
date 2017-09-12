/*
 * main.cpp
 *
 *  Created on: Sep 5, 2017
 *      Author: santiago
 */

#include "Game.hpp"
using namespace mch;

const std::string luaVars = "vars.lua";
int main(){
	Game game;
	game.run();

	return 0;
}

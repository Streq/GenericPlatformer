/*
 * main.cpp
 *
 *  Created on: Sep 5, 2017
 *      Author: santiago
 */

#include <Mocho/Application/Application.hpp>
#include <Mocho/Application/AppState.hpp>

#include "LoadingState.hpp"
#include "Tests/test.hpp"
using namespace mch;


constexpr int sizeVert = 10;
int main(){
	Application app;
	std::unique_ptr<AppState> gameState(new LoadingState);
	app.init(std::move(gameState));
	app.run();
//	testSpriteAnimation();
	return 0;
}

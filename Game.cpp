/*
 * Game.cpp
 *
 *  Created on: Sep 11, 2017
 *      Author: santiago
 */

#include "Game.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "window.hpp"
#include "math.hpp"
#include "text.hpp"
#include "vec2.hpp"
#include "time.hpp"


#include <lua.hpp>
#include <iostream>

constexpr auto luafile="vars.lua";
namespace mch{
void Game::run() {
	init();
	Time deltaTime=Time::Zero;
	Clock clock;
	while(window.isOpen()){
		do{
			deltaTime += clock.restart();
		}while(limitFPS && deltaTime < timePerFrame);

		handle_events();

		do{
			deltaTime -= timePerFrame;
			deltaTime = std::max(deltaTime,sf::Time::Zero);

			update();
		}while(frameskip && deltaTime > timePerFrame);
		render();
	}
}

void Game::init() {

	//lua
	auto state = luaL_newstate();
	if (luaL_loadfile(state, luafile) || lua_pcall(state, 0, 0, 0)) {
		std::cout<<"Error: script not loaded ("<<luafile<<")"<<std::endl;
		state = 0;
	}
	lua_getglobal(state,"winSize");
	lua_getfield(state,-1,"X");
	windowSize.x=lua_tointeger(state,-1);
	lua_pop(state,1);
	lua_getfield(state,-1,"Y");
	windowSize.y=lua_tointeger(state,-1);
	lua_pop(state,1);
	lua_pop(state,1);
	auto resolution = sf::VideoMode::getDesktopMode();
	Vec2i centerPos ((Vec2u(resolution.width,resolution.height)-windowSize)/2u);

	window.create(sf::VideoMode(windowSize.x,windowSize.y), "Pantiforma", sf::Style::Default);
	window.setPosition(centerPos);
	lua_getglobal(state,"frameskip");
	speed = lua_toboolean(state,-1);
	lua_pop(state,1);

	lua_getglobal(state,"limitFPS");
	limitFPS = lua_toboolean(state,-1);
	lua_pop(state,1);

	lua_getglobal(state,"FPS");
	int fps = lua_tointeger(state,-1);
	if(fps>0)
		secondsPerFrame = 1.f/fps;
	timePerFrame = sf::seconds(secondsPerFrame);
	lua_pop(state,1);

	lua_getglobal(state,"player");

	lua_getfield(state,-1,"speed");
	speed = lua_tonumber(state,-1);
	lua_pop(state,1);

	lua_getfield(state,-1,"buoyancy");
	buoyancy = lua_tonumber(state,-1);
	lua_pop(state,1);

	lua_getfield(state,-1,"friction");
	friction = lua_tonumber(state,-1);
	lua_pop(state,1);

	lua_getfield(state,-1,"radius");
	radius = lua_tonumber(state,-1);
	lua_pop(state,1);

	lua_getfield(state,-1,"rebound");
	rebound = lua_tonumber(state,-1);
	lua_pop(state,2);

	lua_getglobal(state,"gravity");

	lua_getfield(state,-1,"X");
	gravity.x = lua_tonumber(state,-1);
	lua_pop(state,1);

	lua_getfield(state,-1,"Y");
	gravity.y = lua_tonumber(state,-1);
	lua_pop(state,1);
	lua_close(state);
	//~lua

	//init
	player.setRadius(radius);
	player.setFillColor(sf::Color::Green);
	player.setOutlineThickness(2.f);
	player.setOutlineColor(sf::Color::Blue);
	player.setOrigin(radius,radius);
	player.setPosition(
		Vec2(windowSize) * 0.5f
	);
	velocity=Vec2{0.f,0.f};
	up=down=left=right=false;
}

void Game::setFPS(int fps) {
}


void Game::update() {
	vecSpeed.y = down - up;
	vecSpeed.x = right - left;
	vecSpeed = vec::normalized(vecSpeed);
	velocity += vecSpeed * (speed * secondsPerFrame);
	velocity += gravity * secondsPerFrame;
	Vec2 topLeft{player.getPosition()-Vec2{radius,radius}};
	Vec2 botRight{player.getPosition()+Vec2{radius,radius}};

	if(topLeft.x <= 0.f){
		if(velocity.x<0.f){
			velocity.x *= rebound;
			velocity.x = copysign(velocity.x, 1.f);
		}
		player.setPosition(0.f+radius, player.getPosition().y);
	}
	if(topLeft.y <= 0.f){
		if(velocity.y < 0.f){
			velocity.y *= rebound;
			velocity.y = copysign(velocity.y, 1.f);
		}
		player.setPosition(player.getPosition().x, 0.f + radius);
	}
	if(botRight.x >= windowSize.x){
		if(velocity.x > 0.f){
			velocity.x *= rebound;
			velocity.x = copysign(velocity.x, -1.f);
		}
		player.setPosition(windowSize.x - radius, player.getPosition().y);
	}
	if(botRight.y >= windowSize.y){
		if(velocity.y > 0.f){
			velocity.y *= rebound;
			velocity.y = copysign(velocity.y, -1.f);
		}
		player.setPosition(player.getPosition().x, windowSize.y - radius);

	}
	player.move(velocity);
	auto norm = vec::normalized(velocity);
	velocity.x = approach(velocity.x, 0.f, copysign(norm.x,1.f)*friction*secondsPerFrame);
	velocity.y = approach(velocity.y, 0.f, copysign(norm.y,1.f)*friction*secondsPerFrame);
	velocity *= (1.f-buoyancy*secondsPerFrame);

}

void Game::render() {
	window.clear();
	window.draw(player);
	window.display();
}

void Game::handle_events() {
	sf::Event e;
	while(window.pollEvent(e)){
		switch(e.type){
			case sf::Event::KeyPressed:{
				auto key = e.key.code;
				switch(key){
					case decltype(key)::W:{
						up=true;
					}break;
					case decltype(key)::A:{
						left=true;
					}break;
					case decltype(key)::D:{
						right=true;
					}break;
					case decltype(key)::S:{
						down=true;
					}break;
				}
			}break;
			case sf::Event::KeyReleased:{
				auto key = e.key.code;
				switch(key){
					case decltype(key)::W:{
						up=false;
					}break;
					case decltype(key)::A:{
						left=false;
					}break;
					case decltype(key)::D:{
						right=false;
					}break;
					case decltype(key)::S:{
						down=false;
					}break;
					case decltype(key)::R:{
						init();
					}break;
				}
			}break;
			case sf::Event::Closed:{
				window.close();
			}break;
		}
	};
}

}

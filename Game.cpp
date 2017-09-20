/*
 * Game.cpp
 *
 *  Created on: Sep 11, 2017
 *      Author: santiago
 */

#include "Game.hpp"

#include <lua.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <Mocho/window.hpp>
#include <Mocho/math.hpp>
#include <Mocho/text.hpp>
#include <Mocho/vec2.hpp>
#include <Mocho/time.hpp>
#include <Mocho/collision.hpp>
#include <Mocho/lua.hpp>

#include <iostream>

constexpr auto luafile="vars.lua";
namespace mch{
void Game::run() {
	init();
	Time deltaTime=Time::Zero;
	Clock clock;
	Clock renderClock;
	int32 stepsUntilRender = stepsPerRender;
	while(window.isOpen()){
		do{
			deltaTime += clock.restart();
		}while(limitFPS && deltaTime < physicsDeltaTime);

		handle_events();
		
		do{
			//decrement deltaTime or set it to 0 depending on frameskip
			deltaTime = std::max
				( (deltaTime-physicsDeltaTime)*static_cast<float>(frameskip)
				, sf::Time::Zero
				)
			;
			update();
			--stepsUntilRender;
		}while(deltaTime > physicsDeltaTime);
		
		if(stepsUntilRender<=0){
			msSinceLastRender = renderClock.restart().asMicroseconds();
			render();
			stepsUntilRender=stepsPerRender;
		}
	}
}

void Game::init() {


	//lua
	lua::Script script;
	script.doFile(luafile);
//	auto state = luaL_newstate();
//	if (luaL_loadfile(state, luafile) || lua_pcall(state, 0, 0, 0)) {
//		std::cout<<"Error: script not loaded ("<<luafile<<")"<<std::endl;
//		state = 0;
//	}
	script.getByFullName("winSize.X",windowSize.x);
	script.getByFullName("winSize.Y",windowSize.y);

	auto resolution = sf::VideoMode::getDesktopMode();
	Vec2i centerPos ((Vec2u(resolution.width,resolution.height)-windowSize)/2u);

	window.create(sf::VideoMode(windowSize.x,windowSize.y), "Pantiforma", sf::Style::Default);
	window.setPosition(centerPos);
	script.getByFullName("frameskip",frameskip);

	int fps=0;
	script.getByFullName("PhysicsFPS",fps);
	if(fps>0)
		physicsDeltaSecs = 1.f/fps;
	physicsDeltaTime = sf::seconds(physicsDeltaSecs);

	script.getByFullName("StepsPerRender",stepsPerRender);
	script.getByFullName("limitFPS",limitFPS);


	script.getByFullName("player.acceleration",charProperties.acceleration);
	script.getByFullName("player.buoyancy",charProperties.buoyancy);
	script.getByFullName("player.friction",charProperties.friction);
	script.getByFullName("player.radius",charProperties.radius);
	script.getByFullName("player.jumpSpeed",charProperties.jumpSpeed);
	script.getByFullName("player.rebound",charProperties.rebound);
	script.getByFullName("gravity.X",charProperties.gravity.x);
	script.getByFullName("gravity.Y",charProperties.gravity.y);
	const char* fontpath;
	script.getByFullName("fpsText.font",fontpath);

	fpsFont.loadFromFile(fontpath);
	fpsText.setFont(fpsFont);
	unsigned charSize;
	script.getByFullName("fpsText.size",charSize);
	fpsText.setCharacterSize(charSize);
	fpsText.setColor(sf::Color::White);
	//~lua

	//init
	player.setRadius(charProperties.radius);
	player.setFillColor(sf::Color::Green);
	player.setOutlineThickness(2.f);
	player.setOutlineColor(sf::Color::Blue);
	player.setOrigin(charProperties.radius,charProperties.radius);
	player.setPosition(
		Vec2(windowSize) * 0.5f
	);
	charInitState.velocity=Vec2{0.f,0.f};
	up=down=left=right=jump=false;

	character.setProperties(charProperties);
	character.setState(charInitState);
}

void Game::setFPS(int fps) {
}


void Game::update() {

	//vecAcceleration.y = down - up;
	if(right)
		character.moveRight();
	if(left)
		character.moveLeft();
	if(up)
		character.moveUp();
	if(down)
		character.moveDown();
	if(jump)
		character.jump();
	//vecAcceleration = vec::normalized(vecAcceleration);
	//auto delta_velocity = (vecAcceleration*acceleration + gravity) * physicsDeltaSecs;
	//velocity += delta_velocity;
	//velocity.y += jumpSpeed*static_cast<float>(jump);
	jump=false;
	//Vec2 topLeft{player.getPosition()-Vec2{radius,radius}};
	//Vec2 botRight{player.getPosition()+Vec2{radius,radius}};
	auto bounds = character.getBoundaries();

	/*
	if(bounds.topleft.x <= 0.f){
		if(velocity.x < 0.f){
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
	*/
	character.update(physicsDeltaSecs);
	player.setPosition(character.getPosition());
}

void Game::render() {
	fpsText.setString(std::to_string(msSinceLastRender));

	window.clear();
	window.draw(player);
	window.draw(fpsText);
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
					case decltype(key)::Space:{
						jump=true;
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
			case sf::Event::LostFocus:{
				up=down=left=right=false;
			};
		}
	};
}

}

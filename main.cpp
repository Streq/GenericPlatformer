/*
 * main.cpp
 *
 *  Created on: Sep 5, 2017
 *      Author: santiago
 */

#include "definitions.hpp"
#include "math.hpp"
#include "text.hpp"
#include "vec2.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

using namespace mch;
constexpr int64 microsPerFrame = 16666;
constexpr float32 secsPerFrame = microsPerFrame / 1.e6f;
constexpr float32 radius = 10.f;
const Time timePerFrame(sf::microseconds(microsPerFrame));
int main(){

	//members
	RenderWindow window(sf::VideoMode(800,600), "Pantiforma", sf::Style::Default);
	sf::CircleShape player(radius);
	Vec2 vecSpeed{0.f,0.f};
	Vec2 velocity{0.f,0.f};
	float32 speed = 15.f;
	float32 buoyancy = 0.f;
	float32 friction = 1.f;
	Vec2 gravity{0.f,5.f};
	bool up,down,left,right;
	Vec2 windowSize{static_cast<Vec2>(window.getSize())};
	//~members

	//init
	player.setFillColor(sf::Color::Green);
	player.setOutlineThickness(2.f);
	player.setOutlineColor(sf::Color::Blue);
	player.setOrigin(radius,radius);
	player.setPosition(
		windowSize * 0.5f
	);
	up=down=left=right=false;
	//~init

	Clock clock;
	Time deltaTime{ Time::Zero };
	while(window.isOpen()){
		do{
			deltaTime += clock.restart();
		}while(deltaTime < timePerFrame);

		//handle_events();
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
					}
				}break;
				case sf::Event::Closed:{
					window.close();
				}break;
			}
		};
		//~handle_events

		do{
			deltaTime -= timePerFrame;

			//update();
			vecSpeed.y = down - up;
			vecSpeed.x = right - left;
			vecSpeed = vec::normalized(vecSpeed);
			velocity += vecSpeed * (speed * secsPerFrame);
			velocity += gravity * secsPerFrame;
			Vec2 topLeft{player.getPosition()-Vec2{radius,radius}};
			Vec2 botRight{player.getPosition()+Vec2{radius,radius}};

			if(topLeft.x <= 0.f){
				velocity.x = copysign(velocity.x, 1.f);
				//player.setPosition(0.f+radius, player.getPosition().y);
			}
			if(topLeft.y <= 0.f){
				velocity.y = copysign(velocity.y, 1.f);
				//player.setPosition(player.getPosition().x, 0.f + radius);
			}
			if(botRight.x >= windowSize.x){
				velocity.x = copysign(velocity.x, -1.f);
				//player.setPosition(windowSize.x - radius, player.getPosition().y);
			}
			if(botRight.y >= windowSize.y){
				velocity.y = copysign(velocity.y, -1.f);
				//player.setPosition(player.getPosition().x, windowSize.y - radius);
			}
			player.move(velocity);
			auto norm = vec::normalized(velocity);
			velocity.x = approach(velocity.x, 0.f, copysign(norm.x,1.f)*friction*secsPerFrame);
			velocity.y = approach(velocity.y, 0.f, copysign(norm.y,1.f)*friction*secsPerFrame);
			velocity *= (1.f-buoyancy*secsPerFrame);
			//~update

		}while(deltaTime > timePerFrame);

		//render();
		window.clear();
		window.draw(player);
		window.display();
		//~render
	};
	return 0;
}

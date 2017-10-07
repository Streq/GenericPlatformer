/*
 * GameState.cpp
 *
 *  Created on: Sep 30, 2017
 *      Author: santiago
 */

#include "GameState.hpp"
#include <cstring>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>

#include <Mocho/Application/AppContext.hpp>
#include <Mocho/collision.hpp>
namespace mch {

void GameState::moveEntities(float dt) {
	//get movement direction
	Vec2f dir =
		vec::normalized(
			Vec2f(
				(m_input.map[Input::right]
				- m_input.map[Input::left]),
				(m_input.map[Input::down]
				- m_input.map[Input::up])));
	//update velocity
	m_pj.velocity = dir * m_pj.speed;
	//update position
	m_pj.position += m_pj.velocity * dt;

}

bool GameState::update() {
	//get dt from settings (could change to being an argument)
	auto& settings = this->getContext().settings;
	float dt =
		float(settings.microseconds_per_step)*1e-6;

	moveEntities(dt);

	{//check collisions
		uint32 i=0;
		auto temp = m_pj.getGlobalAABB();
		for(auto& platform: m_platforms){
			if(collision::aabb_aabb(
					platform.BoundingBox,
					temp))
			{
				m_collisions.push_back({i});
			}
			++i;
		}
	}
	{//resolve collisions
		for(auto& collision: m_collisions){
			auto mod_vel=vec::normalized(m_pj.velocity);
			while(
				collision::aabb_aabb(
					m_pj.getGlobalAABB(),
					m_platforms[collision.id1].BoundingBox))
			{
				m_pj.position-=mod_vel;
			}
		}
		m_collisions.clear();
	}

	return true;

}

void GameState::draw(
		sf::RenderTarget& target,
		sf::RenderStates states) const {

	sf::RectangleShape rect{};
	{//player
		rect.setFillColor(sf::Color::Green);
		rect.setSize(m_pj.BoundingBox.size);
		rect.setPosition(m_pj.position + m_pj.BoundingBox.topleft);
		target.draw(rect,states);
	}
	{//platforms
		for(auto& plat: m_platforms){
			rect.setFillColor(sf::Color::Blue);
			rect.setSize(plat.BoundingBox.size);
			rect.setPosition(plat.BoundingBox.topleft);
			target.draw(rect,states);
		}
	}
}

bool GameState::input(
		const sf::Event& event) {
	switch(event.type){
		case sf::Event::KeyPressed:{
			switch(event.key.code){
				case sf::Keyboard::W:{
					m_input.map[Input::up]=true;
				}break;
				case sf::Keyboard::S:{
					m_input.map[Input::down]=true;
				}break;
				case sf::Keyboard::A:{
					m_input.map[Input::left]=true;
				}break;
				case sf::Keyboard::D:{
					m_input.map[Input::right]=true;
				}break;
			}
		}break;
		case sf::Event::KeyReleased:{
			switch(event.key.code){
				case sf::Keyboard::W:{
					m_input.map[Input::up]=false;
				}break;
				case sf::Keyboard::S:{
					m_input.map[Input::down]=false;
				}break;
				case sf::Keyboard::A:{
					m_input.map[Input::left]=false;
				}break;
				case sf::Keyboard::D:{
					m_input.map[Input::right]=false;
				}break;
			}
		}break;
		case sf::Event::LostFocus:{
			memset(&m_input,false,Input::size);
		}break;
	}
	return true;
}

void GameState::init() {
	m_pj.position = {0.f,0.f};
	m_pj.velocity = {0.f,0.f};
	m_pj.BoundingBox = {
		{0.f,0.f},
		{10.f,10.f}
	};
	m_pj.speed = 100.f;

	Platform plat;
	plat.BoundingBox = {
				{100.f,0.f},
				{10.f,30.f}
			};
	m_platforms.push_back(plat);
	plat.BoundingBox = {
				{200.f,300.f},
				{100.f,20.f}
			};
	m_platforms.push_back(plat);
}

} /* namespace mch */

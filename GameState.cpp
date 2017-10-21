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
#include <Mocho/Collision/collision.hpp>
namespace mch {

void GameState::draw
		( sf::RenderTarget& target
		, sf::RenderStates states
		) const
{
	auto view = target.getView();
	view.setCenter(m_characters[0].m_position);
	target.setView(view);
	sf::RectangleShape shape;
	//wall drawing logic
	shape.setFillColor(sf::Color::Blue);
	shape.setOutlineThickness(1.f);
	for(const auto& wall: m_walls){
		shape.setSize(wall.m_body.size);
		shape.setPosition(wall.m_position+wall.m_body.topleft);
		target.draw(shape);
	}
	//character drawing logic
	shape.setFillColor(sf::Color::Green);
	shape.setOutlineThickness(0.f);
	for(const auto& character: m_characters){
		shape.setSize(character.m_body.size);
		shape.setPosition(character.m_position+character.m_body.topleft);
		target.draw(shape);
	}
}


void GameState::init() {
	memset(m_input, false, input_key::size);
	Entity block
		{ Vec2f(0.f,0.f)//velocity
		, Vec2f(0.f,0.f)//position
		, //aabb
			{ Vec2f(25.f,25.f)//topleft
			, Vec2f(25.f,25.f)//size
			}
		};
	Entity floor
		{ Vec2f(0.f,0.f)//velocity
		, Vec2f(0.f,0.f)//position
		, //aabb
			{ Vec2f(25.f,25.f)//topleft
			, Vec2f(250.f,25.f)//size
			}
		};
	Entity sidewall
		{ Vec2f(0.f,0.f)//velocity
		, Vec2f(0.f,0.f)//position
		, //aabb
			{ Vec2f(25.f,25.f)//topleft
			, Vec2f(25.f,250.f)//size
			}
		};

	auto block_factory = [&block](float32 x, float32 y){
		auto ret = block;
		ret.m_position.x=x;
		ret.m_position.y=y;
		return ret;
	};
	auto wall_factory = [&sidewall](float32 x, float32 y){
		auto ret = sidewall;
		ret.m_position.x=x;
		ret.m_position.y=y;
		return ret;
	};
	auto floor_factory = [&floor](float32 x, float32 y){
		auto ret = floor;
		ret.m_position.x=x;
		ret.m_position.y=y;
		return ret;
	};
	Player mplayer;

	mplayer.m_velocity = Vec2f(0.f,0.f);//velocity
	mplayer.m_position = Vec2f(100.f,100.f);//position
	mplayer.m_body= AABB
		{ Vec2f(0.f,0.f)//topleft
		, Vec2f(10.f,10.f)//size
		};
	m_characters.push_back(
		mplayer
	);
	for(int i = 0; i < 1; ++i){
		m_walls.push_back(wall_factory(0.f, 0.f));//250.f - 25.f * i));
		m_walls.push_back(floor_factory(/*25.f * i*/0.f, 0.f));
		m_walls.push_back(floor_factory(/*25.f * i*/0.f, 250.f));
	}
	m_player_speed = 0.f;
	m_player_acceleration = 1000.f;
}

Vec2f GameState::getInputDirection() {
	return	vec::normalized
	( Vec2f
		( m_input[input_key::right] - m_input[input_key::left]
		, m_input[input_key::down] - m_input[input_key::up]
		)
	);
}



bool GameState::update() {

	float dt = getContext().settings.microseconds_per_step*1e-6;
	//input direction vector
	//move entities
	m_characters[0].m_actions[Player::Action::jump]=m_input[input_key::up];
	m_characters[0].m_actions[Player::Action::moveLeft]=m_input[input_key::left];
	m_characters[0].m_actions[Player::Action::moveRight]=m_input[input_key::right];

	for(auto& e : m_walls){
		e.Entity::update(dt);
	}
	for(auto& e : m_characters){
		e.Player::update(dt);
		e.Entity::update(dt);
	}
	auto& walls = m_walls;

	//detect collisions
	std::for_each(m_characters.begin(),m_characters.end(),[&walls,dt](Player& c){
		std::for_each(walls.begin(),walls.end(),[&c,dt](Entity& w){
			auto step_velocity = c.m_velocity*dt;
			auto init_pos = c.m_position - step_velocity;
			//get global AABBs

			auto c_bod_init = c.m_body;
			c_bod_init.topleft += init_pos;

			auto w_bod = w.m_body;
			w_bod.topleft += w.m_position;

			auto c_bod_end = c.m_body;
			c_bod_end.topleft += c.m_position;

			AABB inter_end;
			AABB inter_init;
			//check whether they collide
			while(collision::aabb_aabb(c_bod_end,w_bod,inter_end)){
				//store intersection before collision
				//to check whether it's horizontal or vertical
				collision::aabb_aabb(c_bod_init,w_bod,inter_init);

				bool horizontal_collision = inter_init.size.x <= 0.f;
				bool vertical_collision = inter_init.size.y <= 0.f;
				//solve diagonal case
				if(horizontal_collision&&vertical_collision){
					//the one who approaches 0 faster determines the collision type
					float x_approach_factor=0.f;
					float y_approach_factor=0.f;
					if(step_velocity.x){
						x_approach_factor = copysign(inter_init.size.x/step_velocity.x,1.f);
					};
					if(step_velocity.y){
						y_approach_factor = copysign(inter_init.size.y/step_velocity.y,1.f);
					};
					horizontal_collision = x_approach_factor>y_approach_factor;
					vertical_collision = !horizontal_collision;

				}
				if(horizontal_collision){
					c.m_position.x -= inter_end.size.x * mch::sign(step_velocity.x);
					c.m_velocity.x = 0.f;
				} else if(vertical_collision){
					c.m_position.y -= inter_end.size.y * mch::sign(step_velocity.y);
					if(step_velocity.y>=0.f){
						c.m_state = Player::State::ground;
					}
					c.m_velocity.y = 0.f;
				}
				c_bod_end.topleft = c.m_position + c.m_body.topleft;
			}
		});
	});

	return true;
}

bool GameState::input(
		const sf::Event& e) {
	switch (e.type){
		case sf::Event::KeyPressed:{
			switch(e.key.code){
				case sf::Keyboard::Key::A:{
					m_input[input_key::left]=true;
				}break;
				case sf::Keyboard::Key::S:{
					m_input[input_key::down]=true;
				}break;
				case sf::Keyboard::Key::D:{
					m_input[input_key::right]=true;
				}break;
				case sf::Keyboard::Key::W:{
					m_input[input_key::up]=true;
				}break;
			}
		}break;
		case sf::Event::KeyReleased:{
			switch(e.key.code){
				case sf::Keyboard::Key::A:{
					m_input[input_key::left]=false;
				}break;
				case sf::Keyboard::Key::S:{
					m_input[input_key::down]=false;
				}break;
				case sf::Keyboard::Key::D:{
					m_input[input_key::right]=false;
				}break;
				case sf::Keyboard::Key::W:{
					m_input[input_key::up]=false;
				}break;
				case sf::Keyboard::Key::R:{
					m_characters[0].m_position=Vec2f(100.f,100.f);
				}break;
			}
		}break;
		case sf::Event::LostFocus:{
			memset(m_input, false, input_key::size);
		}break;
		case sf::Event::Closed:{
			requestClear();
		}break;
	}
	return true;
}


}
/* namespace mch */


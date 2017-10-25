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

#include <algorithm>
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
	m_input_1[input_key::down]=sf::Keyboard::Down;
	m_input_1[input_key::up]=sf::Keyboard::Up;
	m_input_1[input_key::left]=sf::Keyboard::Left;
	m_input_1[input_key::right]=sf::Keyboard::Right;

	m_input_2[input_key::down]=sf::Keyboard::S;
	m_input_2[input_key::up]=sf::Keyboard::W;
	m_input_2[input_key::left]=sf::Keyboard::A;
	m_input_2[input_key::right]=sf::Keyboard::D;

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
	mplayer.m_position = Vec2f(150.f,100.f);//position
	m_characters.push_back(
		mplayer
	);
	m_walls.push_back(wall_factory(0.f, 0.f));
	m_walls.push_back(floor_factory(0.f, 0.f));
	m_walls.push_back(floor_factory(0.f, 250.f));
	m_walls.push_back(floor_factory(500.f, 250.f));
	m_walls.push_back(floor_factory(1000.f, 200.f));
	m_walls.push_back(floor_factory(1600.f, 230.f));



	m_player_speed = 0.f;
	m_player_acceleration = 1000.f;
}



bool GameState::update() {

	float dt = getContext().settings.microseconds_per_step*1e-6;
	//input direction vector
	//move entities
	auto& keyboard = getContext().keyboard;

	auto action1 = [this,&keyboard](Player::Action action, input_key key){
		m_characters[0].m_actions[action]=
				keyboard.checkPressed(m_input_1[key]);
	};
	auto action2 = [this,&keyboard](Player::Action action, input_key key){
		m_characters[1].m_actions[action]=
				keyboard.checkPressed(m_input_2[key]);
	};

	action1(Player::Action::jump,input_key::up);
	action1(Player::Action::moveLeft,input_key::left);
	action1(Player::Action::moveRight,input_key::right);

	action2(Player::Action::jump,input_key::up);
	action2(Player::Action::moveLeft,input_key::left);
	action2(Player::Action::moveRight,input_key::right);


	for(auto& e : m_walls){
		e.Entity::update(dt);
	}
	for(auto& e : m_characters){
		e.Player::update(dt);
		e.Entity::update(dt);
		e.m_state=Player::State::air;
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
		case sf::Event::KeyReleased:{
			switch(e.key.code){
				case sf::Keyboard::Key::R:{
					m_characters[0].m_position=Vec2f(100.f,100.f);
				}break;
			}
		}break;
		case sf::Event::Closed:{
			requestClear();
		}break;
	}

	return true;
}


}
/* namespace mch */


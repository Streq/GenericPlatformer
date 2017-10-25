/*
 * Entity.hpp
 *
 *  Created on: Oct 9, 2017
 *      Author: santiago
 */

#pragma once
#include <Mocho/Graphics/SpriteAnimation.hpp>
#include <Mocho/Collision/collision.hpp>

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

namespace mch {

struct Entity{
	public:
		Vec2f m_velocity;
		Vec2f m_position;
		AABB m_body;

		void update(float dt){m_position += m_velocity*dt;}
};

struct Player : public Entity{
	public:
		enum State{
			ground,
			air
		};
		enum Action{
			moveLeft,
			moveRight,
			jump,
			shoot,
			size
		};
		//gravity
		Vec2f m_gravity = {0.f,500.f};

		//horizontal speed
		float m_walk_acc = 1000.f;

		//initial jump speed
		float m_jump_speed = 300.f;

		//x velocity is multiplied by this every frame
		float m_damping_x = 0.5f;

		//y velocity is multiplied by this every frame
		float m_damping_y = 0.5f;

		//x velocity gets subtracted by this every frame
		float m_friction_x = 200.f;

		//y velocity gets subtracted by this every frame
		float m_friction_y = 0.f;

		State m_state = State::air;
		bool m_actions[Action::size] = {};

		void update(float dt){
			switch(m_state){
				case State::ground:{
					if(m_actions[Action::jump]){
						m_velocity.y -= m_jump_speed;
					}
					if(m_actions[Action::moveLeft]){
						m_velocity.x -= dt * m_walk_acc;
					}
					if(m_actions[Action::moveRight]){
						m_velocity.x += dt * m_walk_acc;
					}
				}break;
				case State::air:{
					if(m_actions[Action::moveLeft]){
						m_velocity.x -= dt * m_walk_acc;
					}
					if(m_actions[Action::moveRight]){
						m_velocity.x += dt * m_walk_acc;
					}
				}break;

			}

			m_velocity += m_gravity*dt;

			m_velocity.x = approach(m_velocity.x,0.f,m_friction_x*dt);
			m_velocity.x *= 1.f-m_damping_x*dt;

			m_velocity.y = approach(m_velocity.y,0.f,m_friction_y*dt);
			m_velocity.y *= 1.f-m_damping_y*dt;
		}
};

} /* namespace mch */

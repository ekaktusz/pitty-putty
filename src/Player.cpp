#include "Player.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <SFML/System/Time.hpp>
#include <spdlog/spdlog.h>

Player::Player() : m_RectangleShape({120, 50}), m_RigidBody(0, 0, 120, 50, true, 1.f)
{
	m_RectangleShape.setPosition(100, 100);
	m_RigidBody.setGravityScale(0.f);
	
	m_RigidBody.setBeginContact([&](bgl::RigidBody* other, sf::Vector2f collisionNormal) {
		beginContact(other, collisionNormal);
	});

	m_RigidBody.setEndContact([&](bgl::RigidBody* other, sf::Vector2f collisionNormal) {
		endContact(other, collisionNormal);
	});
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_RectangleShape);
}

void Player::update(const sf::Time& dt)
{
	syncPhysics();
	m_RectangleShape.setPosition(m_Position.x, m_Position.y);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_Velocity.x = std::min(m_Velocity.x + s_Acceleration * dt.asSeconds(), s_MaxSpeed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_Velocity.x = std::max(m_Velocity.x - s_Acceleration * dt.asSeconds(), -s_MaxSpeed);
	}
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && m_Grounded)
	{
		jump();
	}
	
	applyFriction(dt);
	applyGravity(dt);
}

void Player::handleEvent(const sf::Event& event)
{
}

void Player::syncPhysics()
{
	m_Position.x = m_RigidBody.getPosition().x;
	m_Position.y = m_RigidBody.getPosition().y;
	m_RigidBody.setLinearVelocity({ m_Velocity.x, m_Velocity.y });
}

void Player::applyGravity(const sf::Time& dt)
{
	if (!m_Grounded)
	{
		m_Velocity.y -= s_Gravity * dt.asSeconds();
	}
}

void Player::applyFriction(const sf::Time& dt)
{
	if (m_Velocity.x > 0)
	{
		m_Velocity.x = std::max(m_Velocity.x - s_Friction * dt.asSeconds(), 0.f);
	}
	else if (m_Velocity.x < 0)
	{
		m_Velocity.x = std::min(m_Velocity.x + s_Friction * dt.asSeconds(), 0.f);
	}
}

void Player::beginContact(bgl::RigidBody* rigidBody, sf::Vector2f collisionNormal)
{
	spdlog::info("hellloooo");
	if (collisionNormal.y < 0)
	{
		m_Grounded = true;
		m_Velocity.y = 0;
	}
}

void Player::endContact(bgl::RigidBody* rigidBody, sf::Vector2f collisionNormal)
{
	m_Grounded = false;
}

void Player::jump()
{
	spdlog::info("jumped");
	m_Velocity.y = s_JumpSpeed;
	m_Grounded = false;
}

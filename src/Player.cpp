#include "Player.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <SFML/System/Time.hpp>

Player::Player(b2World& world) : m_RectangleShape({120, 50}), m_RigidBody(0, 0, 120, 50, world)
{
	m_RectangleShape.setPosition(100, 100);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_RectangleShape);
}

void Player::update(const sf::Time& dt)
{
	m_RectangleShape.setPosition(m_Position.x, m_Position.y);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_Velocity.x = std::min(m_Velocity.x + acceleration * dt.asSeconds(), max_speed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		
	}
	else
	{
		if (m_Velocity.x > 0)
		{
			m_Velocity.x = std::max(m_Velocity.x - friction * dt.asSeconds(), 0.f);
		}
		else if (m_Velocity.x < 0)
		{
			m_Velocity.x = std::min(m_Velocity.x + friction * dt.asSeconds(), 0.f);
		}
	}

	syncPhysics();
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
}

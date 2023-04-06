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
	m_RectangleShape.setPosition(x, y);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		dx = std::min(dx + acceleration * dt.asSeconds(), max_speed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		
	}
	else
	{
		if (dx > 0)
		{
			dx = std::max(dx - friction * dt.asSeconds(), 0.f);
		}
		else if (dx < 0)
		{
			dx = std::min(dx + friction * dt.asSeconds(), 0.f);
		}
	}

	syncPhysics();
}

void Player::handleEvent(const sf::Event& event)
{

}

void Player::syncPhysics()
{
	x = m_RigidBody.getPosition().x;
	y = m_RigidBody.getPosition().y;
	m_RigidBody.setLinearVelocity({ dx, dy });
}

void Player::applyGravity(const sf::Time& dt)
{
}

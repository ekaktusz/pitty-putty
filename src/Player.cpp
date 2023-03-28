#include "Player.h"
#include <SFML/Graphics/RenderTarget.hpp>

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
	m_RectangleShape.setPosition(m_RigidBody.getPosition());
}

void Player::handleEvent(const sf::Event& event)
{

}

void Player::syncPhysics()
{
	
}

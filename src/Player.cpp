#include "Player.h"
#include <SFML/Graphics/RenderTarget.hpp>

Player::Player() : m_RectangleShape({120, 50})
{
	m_RectangleShape.setPosition(100, 100);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_RectangleShape);
}

void Player::update(const sf::Time& dt)
{

}

void Player::handleEvent(const sf::Event& event)
{

}

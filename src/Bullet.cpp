#include "Bullet.h"

#include <SFML/Graphics/RenderTarget.hpp>


Bullet::Bullet(float x, float y) : m_RigidBody(x, y, 5, 5), m_BulletShape({ 5.f, 5.f })
{
	initialize();

}

Bullet::Bullet(sf::Vector2f position) : m_RigidBody(position.x, position.y, 5, 5), m_BulletShape({ 5.f, 5.f })
{
	initialize();
}

void Bullet::initialize()
{
	m_RigidBody.setGravityScale(0);
	m_RigidBody.setLinearVelocity({ 50, 0 });

	m_BulletShape.setFillColor(sf::Color::Green);
}

void Bullet::update(const sf::Time& dt)
{
	m_Position.x = m_RigidBody.getPosition().x;
	m_Position.y = m_RigidBody.getPosition().y;
	m_BulletShape.setPosition(m_Position);
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_BulletShape);
}

void Bullet::handleEvent(const sf::Event& event)
{
}

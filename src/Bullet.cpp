#include "Bullet.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <any>
#include <string>
#include <type_traits>

#include <bagla-engine/physics/PhysicsWorld.h>
#include <bagla-engine/physics/RigidBody.h>
#include <spdlog/spdlog.h>

Bullet::Bullet(sf::Vector2f position, sf::Vector2f velocity) : m_Position(position), m_BulletShape({ 5.f, 5.f })
{
	m_RigidBody = bgl::PhysicsWorld::getInstance().newRigidBody(position.x, position.y, 5, 5);
	m_RigidBody->setGravityScale(0);
	m_RigidBody->setLinearVelocity(velocity);
	m_RigidBody->setSensor(true);

	m_RigidBody->setBeginContact([&](bgl::RigidBody* other, sf::Vector2f collisionNormal) { beginContact(other, collisionNormal); });

	m_BulletShape.setFillColor(sf::Color::Green);
}

Bullet::Bullet(Bullet&& otherBullet) noexcept
{
	*this = std::move(otherBullet);
}

Bullet& Bullet::operator=(Bullet&& otherBullet) noexcept
{
	if (this != &otherBullet)
	{
		m_Position = std::move(otherBullet.m_Position);
		m_Size = std::move(otherBullet.m_Size);
		m_Velocity = std::move(otherBullet.m_Velocity);
		m_Duration = std::move(otherBullet.m_Duration);
		m_CurrentAge = std::move(otherBullet.m_CurrentAge);
		m_RigidBody = otherBullet.m_RigidBody;
		m_BulletShape = std::move(otherBullet.m_BulletShape);

		otherBullet.m_RigidBody = nullptr;
	}
	return *this;
}

Bullet::~Bullet()
{
	SPDLOG_INFO("destroying");
	if (m_RigidBody != nullptr)
	{
		bgl::PhysicsWorld::getInstance().destroyRigidBody(m_RigidBody);
	}
}

void Bullet::update(const sf::Time& dt)
{
	m_Position.x = m_RigidBody->getPosition().x;
	m_Position.y = m_RigidBody->getPosition().y;
	m_BulletShape.setPosition(m_Position);
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_BulletShape);
}

bool Bullet::isExpired() const
{
	return m_CurrentAge.getElapsedTime() > m_Duration;
}

void Bullet::beginContact(bgl::RigidBody* rigidBody, sf::Vector2f collisionNormal)
{
	SPDLOG_INFO("Bullet beginContact");

	std::any userCustomData = rigidBody->getUserCustomData();
	if (!userCustomData.has_value())
	{
		SPDLOG_WARN("no usercustomdata in collision");
		return;
	}

	if (userCustomData.type() != typeid(std::string))
	{
		SPDLOG_WARN("invalid type of custom data");
		return;
	}

	std::string userCustomDataString = std::any_cast<std::string>(userCustomData);
	if (userCustomDataString == "solid")
	{
		m_Duration = sf::seconds(0); // deleting it basically
		SPDLOG_INFO("bullet hit solid obstacle");
	}
}

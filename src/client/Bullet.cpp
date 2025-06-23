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

Bullet::Bullet(sf::Vector2f position, sf::Vector2f velocity) : _position(position), _bulletshape({ 5.f, 5.f })
{
	_rigidBody = bgl::PhysicsWorld::getInstance().newRigidBody(position.x, position.y, 5, 5);
	_rigidBody->setGravityScale(0);
	_rigidBody->setLinearVelocity(velocity);
	_rigidBody->setSensor(true);

	_rigidBody->setBeginContact([&](bgl::RigidBody* other, sf::Vector2f collisionNormal) { beginContact(other, collisionNormal); });

	_bulletshape.setFillColor(sf::Color::Green);
}

Bullet::Bullet(Bullet&& otherBullet) noexcept
{
	*this = std::move(otherBullet);
}

Bullet& Bullet::operator=(Bullet&& otherBullet) noexcept
{
	if (this != &otherBullet)
	{
		_position = std::move(otherBullet._position);
		_size = std::move(otherBullet._size);
		_velocity = std::move(otherBullet._velocity);
		_duration = std::move(otherBullet._duration);
		_currentAge = std::move(otherBullet._currentAge);
		_rigidBody = otherBullet._rigidBody;
		_bulletshape = std::move(otherBullet._bulletshape);

		otherBullet._rigidBody = nullptr;
	}
	return *this;
}

Bullet::~Bullet()
{
	SPDLOG_INFO("destroying");
	if (_rigidBody != nullptr)
	{
		bgl::PhysicsWorld::getInstance().destroyRigidBody(_rigidBody);
	}
}

void Bullet::update(const sf::Time& dt)
{
	_position.x = _rigidBody->getPosition().x;
	_position.y = _rigidBody->getPosition().y;
	_bulletshape.setPosition(_position);
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_bulletshape);
}

bool Bullet::isExpired() const
{
	return _currentAge.getElapsedTime() > _duration;
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
		_duration = sf::seconds(0); // deleting it basically
		SPDLOG_INFO("bullet hit solid obstacle");
	}
}

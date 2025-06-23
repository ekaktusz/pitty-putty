#include "Player.h"
#include "BulletManager.h"
#include "RigidBodyType.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <any>
#include <bagla-engine/animation/Animation.h>
#include <bagla-engine/asset-manager/AssetManager.h>
#include <bagla-engine/physics/PhysicsWorld.h>
#include <memory>
#include <physics/RigidBody.h>
#include <spdlog/spdlog.h>
#include <string>

Player::Player()
{
	_rigidBody = bgl::PhysicsWorld::getInstance().newRigidBody(0, 0, 48 * 1.5 - 20, 48 * 1.5 - 20, true, 1.f);
	_rigidBody->setGravityScale(0.f);
	_rigidBody->setBeginContact([&](bgl::RigidBody* other, sf::Vector2f collisionNormal) { beginContact(other, collisionNormal); });

	_rigidBody->setEndContact([&](bgl::RigidBody* other, sf::Vector2f collisionNormal) { endContact(other, collisionNormal); });
	_rigidBody->setUserCustomData(RigidBodyType::Player);

	bgl::AssetManager::getInstance().loadTexture("../../assets/spritesheets/characters/Yellow/Gunner_Yellow_Idle.png", "yellow-idle");
	const sf::Texture& idleAnimationTexture = bgl::AssetManager::getInstance().getTexture("yellow-idle");
	auto idleAnimation = std::make_unique<bgl::Animation>(idleAnimationTexture, sf::Vector2i(48, 48), sf::Vector2i(0, 0), sf::Vector2i(4, 0), sf::seconds(0.1f));

	bgl::AssetManager::getInstance().loadTexture("../../assets/spritesheets/characters/Yellow/Gunner_Yellow_Run.png", "yellow-run");
	const sf::Texture& runningAnimationTexture = bgl::AssetManager::getInstance().getTexture("yellow-run");
	auto runningAnimation = std::make_unique<bgl::Animation>(runningAnimationTexture, sf::Vector2i(48, 48), sf::Vector2i(0, 0), sf::Vector2i(4, 0), sf::seconds(0.1f));

	_animationContainer.addAnimation("idle", std::move(idleAnimation));
	_animationContainer.addAnimation("running", std::move(runningAnimation));
	_animationContainer.setScale(1.5f, 1.5f);
}

Player::~Player()
{
	bgl::PhysicsWorld::getInstance().destroyRigidBody(_rigidBody);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_animationContainer);
}

void Player::update(const sf::Time& dt)
{
	syncPhysics();
	static const sf::Vector2f animationOffset { -10, -10 };
	_animationContainer.setPosition(_position + animationOffset);
	_animationContainer.update(dt);
	updateKeyboard(dt);

	if (_velocity.x != 0)
	{
		_animationContainer.setCurrentAnimation("running");
		_direction = _velocity.x < 0 ? Direction::LEFT : Direction::RIGHT;
	}
	else
	{
		_animationContainer.setCurrentAnimation("idle");
	}

	applyFriction(dt);
	applyGravity(dt);

	if (_direction == Direction::RIGHT)
	{
		_animationContainer.flipHorizontally(false);
	}
	else
	{
		_animationContainer.flipHorizontally(true);
	}
}

void Player::updateKeyboard(const sf::Time& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_velocity.x = std::min(_velocity.x + ACCELERATION * dt.asSeconds(), MAX_SPEED);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		_velocity.x = std::max(_velocity.x - ACCELERATION * dt.asSeconds(), -MAX_SPEED);
	}
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && _Grounded)
	{
		jump();
	}
}

void Player::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::T)
		{
			const sf::Vector2f bulletVelocity { (_direction == Direction::RIGHT ? 1 : -1) * 400.f, 0.f };
			const float bulletXPosition = getCenterPosition().x + (_direction == Direction::RIGHT ? 1 : -1) * 20;
			const sf::Vector2f bulletStartingPosition { bulletXPosition, getCenterPosition().y - 5 };
			BulletManager::getInstance().createBullet(bulletStartingPosition, bulletVelocity);
			SPDLOG_INFO("shoot");
		}
	}
}

sf::Vector2f Player::getCenterPosition() const
{
	return _position + _rigidBody->getSize() / 2.f;
}

void Player::setPosition(sf::Vector2f position)
{
	_position = position;
	_rigidBody->setPosition(position);
}

sf::Vector2f Player::getVelocity() const
{
	return _velocity;
}

void Player::syncPhysics()
{
	// SPDLOG_INFO("vx: " + std::to_string(_velocity.x) + " vy: " + std::to_string(_velocity.y));
	_position.x = _rigidBody->getPosition().x;
	_position.y = _rigidBody->getPosition().y;
	_rigidBody->setLinearVelocity({ _velocity.x, _velocity.y });
}

void Player::applyGravity(const sf::Time& dt)
{
	if (!_Grounded)
	{
		_velocity.y -= GRAVITY * dt.asSeconds();
	}
}

void Player::applyFriction(const sf::Time& dt)
{
	if (_velocity.x > 0)
	{
		_velocity.x = std::max(_velocity.x - FRICTION * dt.asSeconds(), 0.f);
	}
	else if (_velocity.x < 0)
	{
		_velocity.x = std::min(_velocity.x + FRICTION * dt.asSeconds(), 0.f);
	}
}

void Player::beginContact(bgl::RigidBody* rigidBody, sf::Vector2f collisionNormal)
{
	SPDLOG_TRACE("Player beginContact");

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
	if (userCustomDataString != "solid")
	{
		SPDLOG_INFO("collision started with not solid");
		return;
	}

	SPDLOG_TRACE("collision started with solid");

	if (collisionNormal.y < 0)
	{
		_Grounded = true;
		_currentGroundBody = rigidBody;
		_velocity.y = 0;
	}
	else if (collisionNormal.y > 0)
	{
		_velocity.y = 0;
	}
}

void Player::endContact(bgl::RigidBody* rigidBody, sf::Vector2f collisionNormal)
{
	SPDLOG_TRACE("Player endContact");

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
	if (userCustomDataString != "solid")
	{
		SPDLOG_INFO("collision started with not solid");
		return;
	}

	SPDLOG_TRACE("collision ended with solid");
	if (_currentGroundBody == rigidBody)
	{
		_Grounded = false;
	}
}

void Player::jump()
{
	SPDLOG_INFO("jumped");
	_velocity.y = JUMP_SPEED;
	_Grounded = false;
}

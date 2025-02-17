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
	m_RigidBody = bgl::PhysicsWorld::getInstance().newRigidBody(0, 0, 48 * 1.5 - 20, 48 * 1.5 - 20, true, 1.f);
	m_RigidBody->setGravityScale(0.f);
	m_RigidBody->setBeginContact([&](bgl::RigidBody* other, sf::Vector2f collisionNormal) { beginContact(other, collisionNormal); });

	m_RigidBody->setEndContact([&](bgl::RigidBody* other, sf::Vector2f collisionNormal) { endContact(other, collisionNormal); });
	m_RigidBody->setUserCustomData(RigidBodyType::Player);

	bgl::AssetManager::getInstance().loadTexture("../../assets/spritesheets/characters/Yellow/Gunner_Yellow_Idle.png", "yellow-idle");
	const sf::Texture& idleAnimationTexture = bgl::AssetManager::getInstance().getTexture("yellow-idle");
	auto idleAnimation = std::make_unique<bgl::Animation>(idleAnimationTexture, sf::Vector2i(48, 48), sf::Vector2i(0, 0), sf::Vector2i(4, 0), sf::seconds(0.1f));

	bgl::AssetManager::getInstance().loadTexture("../../assets/spritesheets/characters/Yellow/Gunner_Yellow_Run.png", "yellow-run");
	const sf::Texture& runningAnimationTexture = bgl::AssetManager::getInstance().getTexture("yellow-run");
	auto runningAnimation = std::make_unique<bgl::Animation>(runningAnimationTexture, sf::Vector2i(48, 48), sf::Vector2i(0, 0), sf::Vector2i(4, 0), sf::seconds(0.1f));

	m_AnimationContainer.addAnimation("idle", std::move(idleAnimation));
	m_AnimationContainer.addAnimation("running", std::move(runningAnimation));
	m_AnimationContainer.setScale(1.5f, 1.5f);
}

Player::~Player()
{
	bgl::PhysicsWorld::getInstance().destroyRigidBody(m_RigidBody);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_AnimationContainer);
}

void Player::update(const sf::Time& dt)
{
	syncPhysics();
	static const sf::Vector2f animationOffset { -10, -10 };
	m_AnimationContainer.setPosition(m_Position + animationOffset);
	m_AnimationContainer.update(dt);
	updateKeyboard(dt);

	if (m_Velocity.x != 0)
	{
		m_AnimationContainer.setCurrentAnimation("running");
		m_Direction = m_Velocity.x < 0 ? Direction::LEFT : Direction::RIGHT;
	}
	else
	{
		m_AnimationContainer.setCurrentAnimation("idle");
	}

	applyFriction(dt);
	applyGravity(dt);

	if (m_Direction == Direction::RIGHT)
	{
		m_AnimationContainer.flipHorizontally(false);
	}
	else
	{
		m_AnimationContainer.flipHorizontally(true);
	}
}

void Player::updateKeyboard(const sf::Time& dt)
{
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
}

void Player::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::T)
		{
			const sf::Vector2f bulletVelocity { (m_Direction == Direction::RIGHT ? 1 : -1) * 400.f, 0.f };
			const float bulletXPosition = getCenterPosition().x + (m_Direction == Direction::RIGHT ? 1 : -1) * 20;
			const sf::Vector2f bulletStartingPosition { bulletXPosition, getCenterPosition().y - 5 };
			BulletManager::getInstance().createBullet(bulletStartingPosition, bulletVelocity);
			SPDLOG_INFO("shoot");
		}
	}
}

sf::Vector2f Player::getCenterPosition() const
{
	return m_Position + m_RigidBody->getSize() / 2.f;
}

void Player::setPosition(sf::Vector2f position)
{
	m_Position = position;
	m_RigidBody->setPosition(position);
}

sf::Vector2f Player::getVelocity() const
{
	return m_Velocity;
}

void Player::syncPhysics()
{
	// SPDLOG_INFO("vx: " + std::to_string(m_Velocity.x) + " vy: " + std::to_string(m_Velocity.y));
	m_Position.x = m_RigidBody->getPosition().x;
	m_Position.y = m_RigidBody->getPosition().y;
	m_RigidBody->setLinearVelocity({ m_Velocity.x, m_Velocity.y });
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
		m_Grounded = true;
		m_CurrentGroundBody = rigidBody;
		m_Velocity.y = 0;
	}
	else if (collisionNormal.y > 0)
	{
		m_Velocity.y = 0;
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
	if (m_CurrentGroundBody == rigidBody)
	{
		m_Grounded = false;
	}
}

void Player::jump()
{
	SPDLOG_INFO("jumped");
	m_Velocity.y = s_JumpSpeed;
	m_Grounded = false;
}

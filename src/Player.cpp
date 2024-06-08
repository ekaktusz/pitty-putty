#include "Player.h"
#include "RigidBodyType.h"
#include <algorithm>
#include <any>
#include <bagla-engine/animation/Animation.h>
#include <bagla-engine/asset-manager/AssetManager.h>
#include <bagla-engine/physics/PhysicsWorld.h>
#include <physics/RigidBody.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <memory>

Player::Player()
{
	m_RigidBody = bgl::PhysicsWorld::getInstance().newRigidBody(0, 0, 48 * 1.5 - 20, 48 * 1.5 - 20, true, 1.f);
	m_RigidBody->setGravityScale(0.f);
	m_RigidBody->setBeginContact([&](bgl::RigidBody* other, sf::Vector2f collisionNormal) {
		beginContact(other, collisionNormal);
	});

	m_RigidBody->setEndContact([&](bgl::RigidBody* other, sf::Vector2f collisionNormal) {
		endContact(other, collisionNormal);
	});
	m_RigidBody->setUserCustomData(RigidBodyType::Player);

	bgl::AssetManager::getInstance().loadTexture("../../assets/spritesheets/characters/Yellow/Gunner_Yellow_Idle.png", "yellow-idle");
	const sf::Texture& idleAnimationTexture = bgl::AssetManager::getInstance().getTexture("yellow-idle");
	auto idleAnimation = std::make_unique<bgl::Animation>(idleAnimationTexture, sf::Vector2i(48,48), sf::Vector2i(0, 0), sf::Vector2i(4, 0), sf::seconds(0.1f));

	bgl::AssetManager::getInstance().loadTexture("../../assets/spritesheets/characters/Yellow/Gunner_Yellow_Run.png", "yellow-run");
	const sf::Texture& runningAnimationTexture = bgl::AssetManager::getInstance().getTexture("yellow-run");
	auto runningAnimation = std::make_unique<bgl::Animation>(runningAnimationTexture, sf::Vector2i(48, 48), sf::Vector2i(0, 0), sf::Vector2i(4, 0), sf::seconds(0.1f));

	m_AnimationComponent.addAnimation("idle", std::move(idleAnimation));
	m_AnimationComponent.addAnimation("running", std::move(runningAnimation));
	m_AnimationComponent.setScale(1.5f, 1.5f);
}

Player::~Player()
{
	bgl::PhysicsWorld::getInstance().destroyRigidBody(m_RigidBody);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_AnimationComponent);

	for (const auto& bullet : m_Bullets)
	{
		target.draw(*bullet);
	}
}

void Player::update(const sf::Time& dt)
{
	syncPhysics();
	static const sf::Vector2f animationOffset{-10, -10};
	m_AnimationComponent.setPosition(m_Position + animationOffset);
	m_AnimationComponent.update(dt);
	updateKeyboard(dt);

	if (m_Velocity.x != 0)
	{
		m_AnimationComponent.setCurrentAnimation("running");
		m_Direction = m_Velocity.x < 0 ? Direction::LEFT : Direction::RIGHT;
	}
	else
	{
		m_AnimationComponent.setCurrentAnimation("idle");
	}

	applyFriction(dt);
	applyGravity(dt);

	if (m_Direction == Direction::RIGHT)
	{
		m_AnimationComponent.flipHorizontally(false);
	}
	else
	{
		m_AnimationComponent.flipHorizontally(true);
	}

	for (auto& bullet : m_Bullets)
	{
		bullet->update(dt);
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
			m_Bullets.push_back(new Bullet(m_Position.x + m_RigidBody->getSize().x + 30, m_Position.y + m_RigidBody->getSize().y/2, Bullet::Direction::RIGHT));
			spdlog::info("shoot");
		}
	}
}

sf::Vector2f Player::getCenterPosition() const
{
	return m_Position + m_RigidBody->getSize() / 2.f;
}

void Player::syncPhysics()
{
	// spdlog::info("vx: " + std::to_string(m_Velocity.x) + " vy: " + std::to_string(m_Velocity.y));
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
	spdlog::info("Player beginContact");

	std::any userCustomData = rigidBody->getUserCustomData();
	if (!userCustomData.has_value())
	{
		spdlog::info("collision started with not solid");
		return;
		/*if (userCustomData.type() == typeid(std::string))
		{
			std::string userCustomDataString = std::any_cast<std::string>(userCustomData);
			if (userCustomDataString != "solid")
			{
				spdlog::info("collision started with not solid");
				return;
			}
		}*/
	}

	spdlog::info("collision started with solid");

	if (collisionNormal.y < 0)
	{
		m_Grounded = true;
		m_Velocity.y = 0;
	}
	else if (collisionNormal.y > 0)
	{
		m_Velocity.y = 0;
	}
}

void Player::endContact(bgl::RigidBody* rigidBody, sf::Vector2f collisionNormal)
{
	spdlog::info("Player endContact");

	std::any userCustomData = rigidBody->getUserCustomData();
	if (!userCustomData.has_value())
	{
		spdlog::info("collision ended with not solid");
		return;
		/*if (userCustomData.type() == typeid(std::string))
		{
			std::string userCustomDataString = std::any_cast<std::string>(userCustomData);
			if (userCustomDataString != "solid")
			{
				spdlog::info("collision ended with not solid");
				return;
			}
		}*/
	}
	spdlog::info("collision ended with solid");
	m_Grounded = false;
}

void Player::jump()
{
	spdlog::info("jumped");
	m_Velocity.y = s_JumpSpeed;
	m_Grounded = false;
}

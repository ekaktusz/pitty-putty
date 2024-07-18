#include "BulletManager.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <spdlog/spdlog.h>

#include <sstream>
#include <string>

void BulletManager::update(const sf::Time& dt)
{
	m_Bullets.erase(
		std::remove_if(m_Bullets.begin(), m_Bullets.end(),
			[](const auto& b) { return b->isExpired(); }),
		m_Bullets.end());

	for (auto& bullet : m_Bullets)
	{
		bullet->update(dt);
	}
}

void BulletManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& bullet : m_Bullets)
	{
		target.draw(*bullet);
	}
}

void BulletManager::handleEvent(const sf::Event& event)
{
	for (auto& bullet : m_Bullets)
	{
		bullet->handleEvent(event);
	}
}

void BulletManager::createBullet(sf::Vector2f startingPosition, sf::Vector2f velocity)
{
	m_Bullets.push_back(std::make_unique<Bullet>(startingPosition, velocity));
}

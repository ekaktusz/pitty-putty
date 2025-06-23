#include "BulletManager.h"

#include "Bullet.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <memory>
#include <vector>

void BulletManager::update(const sf::Time& dt)
{

	std::erase_if(_bullets, [](const auto& b) { return b->isExpired(); });

	for (auto& bullet : _bullets)
	{
		bullet->update(dt);
	}
}

void BulletManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& bullet : _bullets)
	{
		target.draw(*bullet);
	}
}

void BulletManager::createBullet(sf::Vector2f startingPosition, sf::Vector2f velocity)
{
	_bullets.push_back(std::make_unique<Bullet>(startingPosition, velocity));
}

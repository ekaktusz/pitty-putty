#pragma once

#include "Bullet.h"
#include "GameObject.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>

#include <memory>
#include <vector>

namespace sf
{
class Time;
class Event;
class RenderTarget;
}

class BulletManager : public bgl::GameObject, public sf::Drawable
{
public:
	static BulletManager& getInstance()
	{
		static BulletManager instance;
		return instance;
	}

	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void createBullet(sf::Vector2f startingPosition, sf::Vector2f velocity);

private:
	BulletManager() = default;

private:
	std::vector<std::unique_ptr<Bullet>> _bullets;
};
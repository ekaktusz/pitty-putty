#pragma once

#include <bagla-engine/physics/RigidBody.h>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <bagla-engine/GameObject.h>
#include <SFML/Graphics/CircleShape.hpp>

class Bullet : public bgl::GameObject
{
public:
	Bullet(sf::Vector2f position, sf::Vector2f velocity);
	Bullet(const Bullet& otherBullet) = delete;
	Bullet& operator=(const Bullet&) = delete;
	Bullet& operator=(Bullet&& otherBullet);
	Bullet(Bullet&& otherBullet) noexcept;
	~Bullet();

	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleEvent(const sf::Event& event) override;

private:
	sf::Vector2f m_Position;
	sf::Vector2f m_Size;
	sf::Vector2f m_Velocity;
	
	sf::Time m_Duration;
	sf::Clock m_CurrentAge;

	bgl::RigidBody* m_RigidBody;

	sf::RectangleShape m_BulletShape;
};

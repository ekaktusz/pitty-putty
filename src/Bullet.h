#pragma once

#include <bagla-engine/GameObject.h>
#include <bagla-engine/physics/RigidBody.h>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class Bullet : public bgl::GameObject
{
public:
	Bullet(sf::Vector2f position, sf::Vector2f velocity);
	Bullet(const Bullet& otherBullet) = delete;
	Bullet& operator=(const Bullet&) = delete;
	Bullet& operator=(Bullet&& otherBullet) noexcept;
	Bullet(Bullet&& otherBullet) noexcept;
	virtual ~Bullet();

	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleEvent(const sf::Event& event) override;

	bool isExpired() const;

	void beginContact(bgl::RigidBody* rigidBody, sf::Vector2f collisionNormal);

private:
	sf::Vector2f m_Position;
	sf::Vector2f m_Size;
	sf::Vector2f m_Velocity;

	sf::Time m_Duration = sf::seconds(5.f);
	sf::Clock m_CurrentAge;

	bgl::RigidBody* m_RigidBody;

	sf::RectangleShape m_BulletShape;
};

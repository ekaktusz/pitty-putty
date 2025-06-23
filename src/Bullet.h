#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <bagla-engine/GameObject.h>
#include <bagla-engine/physics/RigidBody.h>

class Bullet : public bgl::GameObject, public sf::Drawable
{
public:
	Bullet(sf::Vector2f position, sf::Vector2f velocity);
	Bullet(const Bullet& otherBullet) = delete;
	Bullet& operator=(const Bullet&) = delete;
	Bullet& operator=(Bullet&& otherBullet) noexcept;
	Bullet(Bullet&& otherBullet) noexcept;
	~Bullet() override;

	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	bool isExpired() const;

	void beginContact(bgl::RigidBody* rigidBody, sf::Vector2f collisionNormal);

private:
	sf::Vector2f _position;
	sf::Vector2f _size;
	sf::Vector2f _velocity;

	sf::Time _duration = sf::seconds(5.f);
	sf::Clock _currentAge;

	bgl::RigidBody* _rigidBody;

	sf::RectangleShape _bulletshape;
};

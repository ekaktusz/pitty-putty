#pragma once

#include <bagla-engine/physics/RigidBody.h>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <bagla-engine/GameObject.h>
#include <SFML/Graphics/CircleShape.hpp>

class Bullet : public bgl::GameObject
{
public:
	enum class Direction {LEFT, RIGHT};

	Bullet(float x, float y, Direction direction);
	Bullet(sf::Vector2f position, Direction direction);

	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleEvent(const sf::Event& event) override;

private:
	void initialize(Direction direction);

private:
	sf::Vector2f m_Position;
	sf::Vector2f m_Size;
	sf::Vector2f m_Velocity;
	
	sf::Time m_Duration;
	sf::Clock m_CurrentAge;

	bgl::RigidBody m_RigidBody;

	sf::RectangleShape m_BulletShape;
};
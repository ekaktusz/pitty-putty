#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <bagla-engine/GameObject.h>
#include <bagla-engine/physics/RigidBody.h>

class Player : public bgl::GameObject
{
public:
	Player(b2World& world);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(const sf::Time& dt) override;
	void handleEvent(const sf::Event& event) override;

private:
	void syncPhysics();
	void applyGravity(const sf::Time& dt);

private:
	float x = 0;
	float y = 0;
	float dx = 0;
	float dy = 0;
	float acceleration = 4000;
	float max_speed = 200;
	float friction = 3500;

	sf::RectangleShape m_RectangleShape;
	bgl::RigidBody m_RigidBody;
};
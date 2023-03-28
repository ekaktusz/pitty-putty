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

private:
	sf::RectangleShape m_RectangleShape;
	bgl::RigidBody m_RigidBody;
};
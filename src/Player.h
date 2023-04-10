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
	void applyFriction(const sf::Time& dt);
	void onContact(bgl::RigidBody* rigidBody, sf::Vector2f collisionNormal);

private:
	sf::Vector2f m_Position;
	sf::Vector2f m_Velocity;
	bool m_Grounded = false;

	static constexpr float s_Acceleration = 400;
	static constexpr float s_MaxSpeed = 20;
	static constexpr float s_Friction = 350;
	static constexpr float s_Gravity = 100;

	sf::RectangleShape m_RectangleShape;
	bgl::RigidBody m_RigidBody;
};
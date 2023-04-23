#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <bagla-engine/GameObject.h>
#include <bagla-engine/physics/RigidBody.h>

class Player : public bgl::GameObject
{
public:
	Player();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(const sf::Time& dt) override;
	void handleEvent(const sf::Event& event) override;

	sf::Vector2f getCenterPosition() const;

private:
	void syncPhysics();
	void applyGravity(const sf::Time& dt);
	void applyFriction(const sf::Time& dt);
	void beginContact(bgl::RigidBody* rigidBody, sf::Vector2f collisionNormal);
	void endContact(bgl::RigidBody* rigidBody, sf::Vector2f collisionNormal);

	void jump();

private:
	sf::Vector2f m_Position;
	sf::Vector2f m_Velocity;
	bool m_Grounded = false;

	static constexpr float s_Acceleration = 4000 * 1.5;
	static constexpr float s_MaxSpeed = 200 * 1.5;
	static constexpr float s_Friction = 3500 * 1.5;
	static constexpr float s_Gravity = 1000 * 1.5;
	static constexpr float s_JumpSpeed = 500 * 1.5;

	sf::RectangleShape m_RectangleShape;
	bgl::RigidBody m_RigidBody;
};
#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include <bagla-engine/EventHandler.h>
#include <bagla-engine/GameObject.h>
#include <bagla-engine/animation/AnimationContainer.h>
#include <bagla-engine/physics/RigidBody.h>

#include <memory>

class Player : public bgl::GameObject, public bgl::EventHandler, public sf::Drawable
{
public:
	Player();
	~Player();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(const sf::Time& dt) override;
	void handleEvent(const sf::Event& event) override;

	sf::Vector2f getCenterPosition() const;
	void setPosition(sf::Vector2f position);

	sf::Vector2f getVelocity() const;

private:
	void syncPhysics();
	void applyGravity(const sf::Time& dt);
	void applyFriction(const sf::Time& dt);
	void beginContact(bgl::RigidBody* rigidBody, sf::Vector2f collisionNormal);
	void endContact(bgl::RigidBody* rigidBody, sf::Vector2f collisionNormal);

	void updateKeyboard(const sf::Time& dt);

	void jump();

private:
	static constexpr float ACCELERATION = 4000 * 1.5;
	static constexpr float MAX_SPEED = 200 * 1.5;
	static constexpr float FRICTION = 3500 * 1.5;
	static constexpr float GRAVITY = 1000 * 1.5;
	static constexpr float JUMP_SPEED = 500 * 1.5;

	sf::Vector2f _position;
	sf::Vector2f _velocity;
	bool _Grounded = false;

	bgl::RigidBody* _rigidBody;
	bgl::RigidBody* _currentGroundBody;

	enum class Direction
	{
		LEFT,
		RIGHT
	};
	Direction _direction = Direction::RIGHT;

	bgl::AnimationContainer _animationContainer;
};

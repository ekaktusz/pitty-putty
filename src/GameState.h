#pragma once

#include "Player.h"
#include "StarBackground.h"

#include <bagla-engine/camera/Camera.h>
#include <bagla-engine/states/State.h>

#include <SFML/Graphics/Text.hpp>

#include "DialogBox.h"

namespace bgl
{
class Map;
class PhysicsWorld;
class StateManager;
}

class BulletManager;

class GameState : public bgl::State
{
public:
	GameState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow);
	~GameState();

	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleEvent(const sf::Event& event) override;

	void onResume() override;
	void onPause() override;
	void onStart() override;

private:
	void loadAssets();

	sf::Vector2f getPlayerStartingPosition() const;

private:
	bgl::Camera _camera;

	Player _player1;
	bgl::Map* _map;

	bgl::PhysicsWorld& _physicsWorld;

	mutable sf::Text _fpsCounter;
	mutable unsigned int _frameCount;
	mutable sf::Clock _fpsClock;

	StarBackground _starBackground;
};
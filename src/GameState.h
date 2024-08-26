#pragma once

#include "Player.h"

#include <bagla-engine/states/State.h>
#include <bagla-engine/camera/Camera.h>
#include "StarBackground.h"
#include <SFML/Graphics/Text.hpp>

namespace bgl
{
	class Map;
	class PhysicsWorld;
}

class BulletManager;

class GameState : public bgl::State
{
public:
	GameState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow);
	~GameState();

	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	void handleEvent(const sf::Event& event) override;

	void onResume() override;
	void onPause() override;
	void onStart() override;

private:
	void loadAssets();

	sf::Vector2f getPlayerStartingPosition() const;

private:
	bgl::Camera m_Camera;

	Player m_Player1;
	bgl::Map* m_Map;

	bgl::PhysicsWorld& m_PhysicsWorld;

	sf::Text m_fpsCounter;

	StarBackground m_starBackground;
};
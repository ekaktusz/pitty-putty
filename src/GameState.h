#pragma once

#include "Player.h"

#include <bagla-engine/states/State.h>
#include <bagla-engine/camera/Camera.h>


namespace bgl
{
	class Map;
	class PhysicsWorld;
}

class GameState : public bgl::State
{
public:
	GameState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow);
	~GameState();

	void update(const sf::Time& dt) override;
	void draw() const override;
	void handleEvent(const sf::Event& event) override;

	void onResume() override;
	void onPause() override;

private:
	void loadAssets();

private:
	bgl::Camera m_Camera;

	Player m_Player1;
	bgl::Map* m_Map;

	bgl::PhysicsWorld& m_PhysicsWorld;
};
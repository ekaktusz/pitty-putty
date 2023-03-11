#pragma once

#include <bagla-engine/states/State.h>
#include "Player.h"

namespace bgl
{
	class Map;
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
	Player m_Player1;
	bgl::Map* m_Map;
};
#pragma once

#include <bagla-engine/states/State.h>

namespace bgl
{
	class StateManager;
}

class MenuState : public bgl::State
{
public:
	MenuState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow) : State(stateManager, renderWindow) {}
	void update(const sf::Time& dt) override;
	void draw() const override;
	void handleEvent(const sf::Event& event) override;
};
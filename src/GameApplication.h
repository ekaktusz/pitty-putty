#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <bagla-engine/states/StateManager.h>
#include <string>

class GameApplication
{
public:
	static constexpr unsigned int WINDOW_WIDTH = 1270;
	static constexpr unsigned int WINDOW_HEIGHT = 720;
	const std::string WINDOW_NAME = "PittyPutty";

	GameApplication();
	void run();

private:
	sf::RenderWindow _renderWindow;
	bgl::StateManager _stateManager;
};

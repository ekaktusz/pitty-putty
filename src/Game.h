#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <bagla-engine/states/StateManager.h>

class Game
{
public:
	static constexpr unsigned int WINDOW_WIDTH = 1270;
	static constexpr unsigned int WINDOW_HEIGHT = 720;
	const std::string WINDOW_NAME = "PittyPutty";

	Game();
	void run();

private:
	sf::RenderWindow m_RenderWindow;
	bgl::StateManager m_StateManager;
};

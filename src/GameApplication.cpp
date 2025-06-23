#include "GameApplication.h"

#include "MenuState.h"

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include <memory>
#include <type_traits>

GameApplication::GameApplication() :
	_renderWindow { sf::VideoMode(GameApplication::WINDOW_WIDTH, GameApplication::WINDOW_HEIGHT), GameApplication::WINDOW_NAME, sf::Style::Titlebar | sf::Style::Close },
	_stateManager(_renderWindow)
{
	std::unique_ptr<MenuState> menuState = std::make_unique<MenuState>(_stateManager, _renderWindow);
	_stateManager.pushState(std::move(menuState));
}

void GameApplication::run()
{
	sf::Event event;
	sf::Clock clock;
	sf::Time timeSinceLastUpdate { sf::Time::Zero };
	const sf::Time frameTime = sf::seconds(1.f / 60.f); // max fps: 60

	while (_renderWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > frameTime)
		{
			timeSinceLastUpdate -= frameTime;
			while (_renderWindow.pollEvent(event))
			{
				_stateManager.handleEvent(event);
			}
			_stateManager.update(frameTime);
		}
		_stateManager.draw();
	}
}

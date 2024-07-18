#include "Game.h"
#include "MenuState.h"
#include <memory>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

Game::Game() : 
	m_RenderWindow{sf::VideoMode(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT), Game::WINDOW_NAME, sf::Style::Titlebar | sf::Style::Close },
	m_StateManager(m_RenderWindow)
{
	std::unique_ptr<MenuState> menuState = std::make_unique<MenuState>(m_StateManager, m_RenderWindow);
	m_StateManager.pushState(std::move(menuState));
}

void Game::run()
{
	sf::Event event;
	sf::Clock clock;
	sf::Time timeSinceLastUpdate{ sf::Time::Zero };
	const sf::Time frameTime = sf::seconds(1.f / 60.f); // max fps: 60
	
	while (m_RenderWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > frameTime)
		{
			timeSinceLastUpdate -= frameTime;
			while (m_RenderWindow.pollEvent(event))
			{
				m_StateManager.handleEvent(event);
			}
			m_StateManager.update(frameTime);
		}
		m_StateManager.draw();
	}
}

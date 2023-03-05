#include "GameState.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

GameState::GameState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow) : bgl::State(stateManager, renderWindow)
{

}

GameState::~GameState()
{

}

void GameState::update(const sf::Time& dt)
{

}

void GameState::draw() const
{
	m_RenderWindow.clear();
	m_RenderWindow.draw(m_Player1);
	m_RenderWindow.display();
}

void GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		m_RenderWindow.close();
	}
}

void GameState::onResume()
{

}

void GameState::onPause()
{

}

#include "GameState.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <bagla-engine/asset-manager/AssetManager.h>
#include <bagla-engine/map/Map.h>
#include <bagla-engine/map/TileLayer.h>
#include <bagla-engine/states/StateManager.h>
#include "PauseState.h"

GameState::GameState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow) : bgl::State(stateManager, renderWindow), m_Map(nullptr)
{
	loadAssets();
	m_Map = &bgl::AssetManager::getInstance().getMap("testmap");
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
	m_RenderWindow.draw(m_Map->getTileLayer("backlayer"));
	m_RenderWindow.display();
}

void GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		m_RenderWindow.close();
	}
	else if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			std::unique_ptr<PauseState> pauseState = std::make_unique<PauseState>(m_StateManager, m_RenderWindow);
			m_StateManager.pushState(std::move(pauseState));
		}
	}
}

void GameState::onResume()
{

}

void GameState::onPause()
{

}

void GameState::loadAssets()
{
	bgl::AssetManager::getInstance().loadMap("../../assets/maps/testmap.tmx", "testmap");
}

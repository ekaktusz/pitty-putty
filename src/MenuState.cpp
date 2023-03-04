#include "MenuState.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <bagla-engine/asset-manager/AssetManager.h>
#include <spdlog/spdlog.h>
#include <filesystem>

MenuState::MenuState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow) : State(stateManager, renderWindow) 
{
	loadAssets();
	m_BackgroundTexture = bgl::AssetManager::getInstance().getTexture("background0");
	m_BackgroundSprite.setTexture(m_BackgroundTexture);
}

void MenuState::loadAssets()
{
	bgl::AssetManager::getInstance().loadTexture("../../assets/background/bg0.png", "background0");
}

void MenuState::update(const sf::Time& dt)
{

}

void MenuState::draw() const
{
	m_RenderWindow.clear();
	m_RenderWindow.draw(m_BackgroundSprite);
	m_RenderWindow.display();
}


void MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		m_RenderWindow.close();
	}
}
#include "SettingsState.h"
#include <bagla-engine/states/StateManager.h>
#include <spdlog/spdlog.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <bagla-engine/asset-manager/AssetManager.h>
#include <SFML/System/Time.hpp>
#include <SFML/Audio/Music.hpp>

SettingsState::SettingsState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow)
	: bgl::State(stateManager, renderWindow)
{
	spdlog::info("state created");
	m_BackgroundMusic = &bgl::AssetManager::getInstance().getMusic("menuMusic");
	m_BackgroundMusic->play();
}

void SettingsState::update(const sf::Time& dt)
{
	
}

void SettingsState::draw() const
{

}

void SettingsState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		m_RenderWindow.close();
	}
}

#include "MenuState.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <bagla-engine/asset-manager/AssetManager.h>
#include <spdlog/spdlog.h>
#include <filesystem>

MenuState::MenuState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow) : State(stateManager, renderWindow) 
{
	loadAssets();
	m_BackgroundTexture = bgl::AssetManager::getInstance().getTexture("menuBackground");
	m_BackgroundSprite.setTexture(m_BackgroundTexture);
	m_BackgroundMusic = &bgl::AssetManager::getInstance().getMusic("menuMusic");
	m_BackgroundMusic->play();
	m_GameTitle.setFont(bgl::AssetManager::getInstance().getFont("pixelFont"));
	m_GameTitle.setCharacterSize(100);
	m_GameTitle.setString("Pitty Putty");
	m_GameTitle.setColor(sf::Color::White);
	m_GameTitle.setOutlineColor(sf::Color::Black);
	m_GameTitle.setOutlineThickness(5);

	sf::Vector2f center{m_GameTitle.getGlobalBounds().width / 2.f, m_GameTitle.getGlobalBounds().height / 2.f};
	sf::Vector2f localBounds{ center.x + m_GameTitle.getLocalBounds().left, center.y + m_GameTitle.getLocalBounds().top };
	sf::Vector2f rounded{std::round(localBounds.x), std::round(localBounds.y)};
	m_GameTitle.setOrigin(rounded);
	m_GameTitle.setPosition(m_RenderWindow.getSize().x / 2, 100 );


	//m_GameTitle.setPosition(350, 10);
}

void MenuState::loadAssets()
{
	bgl::AssetManager::getInstance().loadTexture("../../assets/background/bg0.png", "menuBackground");
	bgl::AssetManager::getInstance().loadMusic("../../assets/music/ludumdare38/track10.wav", "menuMusic");
	bgl::AssetManager::getInstance().loadFont("../../assets/fonts/pixel_art_font.ttf", "pixelFont");
}

void MenuState::update(const sf::Time& dt)
{

}

void MenuState::draw() const
{
	m_RenderWindow.clear();
	m_RenderWindow.draw(m_BackgroundSprite);
	m_RenderWindow.draw(m_GameTitle);
	m_RenderWindow.display();
}


void MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		m_RenderWindow.close();
	}
}
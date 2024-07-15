#include "GameState.h"
#include "MenuState.h"
#include "SettingsState.h"

#include <bagla-engine/asset-manager/AssetManager.h>
#include <bagla-engine/states/State.h>
#include <bagla-engine/states/StateManager.h>

#include <spdlog/spdlog.h>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <cmath>
#include <memory>
#include <string>
#include <type_traits>

MenuState::MenuState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow) : 
	bgl::State(stateManager, renderWindow),
	m_StartButton(renderWindow),
	m_SettingsButton(renderWindow),
	m_QuitButton(renderWindow)
{
	loadAssets();
	m_BackgroundTexture = bgl::AssetManager::getInstance().getTexture("menuBackground");
	m_BackgroundSprite.setTexture(m_BackgroundTexture);
	m_BackgroundMusic = &bgl::AssetManager::getInstance().getMusic("menuMusic");
	m_BackgroundMusic->setVolume(50);
	m_BackgroundMusic->setLoop(true);
	m_BackgroundMusic->play();
	m_GameTitle.setFont(bgl::AssetManager::getInstance().getFont("pixelFont"));
	m_GameTitle.setCharacterSize(100);
	m_GameTitle.setString("Pitty Putty");
	m_GameTitle.setFillColor(sf::Color::White);
	m_GameTitle.setOutlineColor(sf::Color::Black);
	m_GameTitle.setOutlineThickness(5);

	sf::Vector2f center{m_GameTitle.getGlobalBounds().width / 2.f, m_GameTitle.getGlobalBounds().height / 2.f};
	sf::Vector2f localBounds{ center.x + m_GameTitle.getLocalBounds().left, center.y + m_GameTitle.getLocalBounds().top };
	sf::Vector2f rounded{std::round(localBounds.x), std::round(localBounds.y)};
	m_GameTitle.setOrigin(rounded);
	m_GameTitle.setPosition(m_RenderWindow.getSize().x / 2.f, 100.f );

	m_StartButton.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	m_StartButton.setSize({ 400, 50 });
	m_StartButton.setPosition({ m_RenderWindow.getSize().x / 2.f - m_StartButton.getSize().x / 2.f , 300.f });
	m_StartButton.setString("start game");
	m_StartButton.setCornerRadius(10.f);
	m_StartButton.setActionTodo([&]() {
		spdlog::info("Switch to GameState: Starting the game");
		m_BackgroundMusic->pause();
		std::unique_ptr<GameState> gameState = std::make_unique<GameState>(m_StateManager, m_RenderWindow);
		m_StateManager.pushState(std::move(gameState));
	});

	m_SettingsButton.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	m_SettingsButton.setSize({ 400, 50 });
	m_SettingsButton.setPosition({ m_RenderWindow.getSize().x / 2 - m_StartButton.getSize().x / 2 , 360 });
	m_SettingsButton.setString("settings");
	m_SettingsButton.setActionTodo([&]() {
		spdlog::debug("settings button pressed.");
		std::unique_ptr<SettingsState> settingsState = std::make_unique<SettingsState>(m_StateManager, m_RenderWindow);
		m_StateManager.pushState(std::move(settingsState));
	});

	m_QuitButton.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	m_QuitButton.setSize({ 400, 50 });
	m_QuitButton.setPosition({ m_RenderWindow.getSize().x / 2.f - m_QuitButton.getSize().x / 2.f , 420.f });
	m_QuitButton.setString("quit game");
	m_QuitButton.setActionTodo([&]() {
		spdlog::info("Quit game :(");
		m_RenderWindow.close();
	});

	// m_GameTitle.setPosition(350, 10);

	m_OpenTransition.start();

	spdlog::info("sx" + std::to_string(m_RenderWindow.getSize().x) + " sy:" + std::to_string(m_RenderWindow.getSize().y));
}

void MenuState::loadAssets()
{
	bgl::AssetManager::getInstance().loadTexture("../../assets/background/bg0.png", "menuBackground");
	bgl::AssetManager::getInstance().loadMusic("../../assets/music/ludumdare38/track10.wav", "menuMusic");
	bgl::AssetManager::getInstance().loadFont("../../assets/fonts/pixel_art_font.ttf", "pixelFont");
	bgl::AssetManager::getInstance().loadFont("../../assets/fonts/upheaval.ttf", "upheaval");
}

void MenuState::update(const sf::Time& dt)
{
	m_StartButton.update(dt);
	m_SettingsButton.update(dt);
	m_QuitButton.update(dt);
	m_OpenTransition.update(dt);
}

void MenuState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.clear();
	target.draw(m_BackgroundSprite);
	target.draw(m_GameTitle);
	target.draw(m_StartButton);
	target.draw(m_SettingsButton);
	target.draw(m_QuitButton);
	target.draw(m_OpenTransition);
}


void MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		m_RenderWindow.close();
	}
	m_StartButton.handleEvent(event);
	m_SettingsButton.handleEvent(event);
	m_QuitButton.handleEvent(event);
}

void MenuState::onResume()
{
	if (m_BackgroundMusic->getStatus() != sf::Music::Status::Playing) 
	{
		m_BackgroundMusic->play();
	}
}

void MenuState::onPause()
{
}

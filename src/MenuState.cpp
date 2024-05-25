#include "MenuState.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <bagla-engine/asset-manager/AssetManager.h>
#include <spdlog/spdlog.h>
#include <filesystem>
#include "GameState.h"
#include <bagla-engine/states/StateManager.h>
#include "SettingsState.h"

MenuState::MenuState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow) : 
	bgl::State(stateManager, renderWindow),
	m_StartButton(renderWindow),
	m_SettingsButton(renderWindow),
	m_QuitButton(renderWindow),
	m_VolumeSlider(renderWindow)
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

	m_StartButton.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	m_StartButton.setSize({ 400, 50 });
	m_StartButton.setPosition({ m_RenderWindow.getSize().x / 2 - m_StartButton.getSize().x / 2 , 300 });
	m_StartButton.setString("start game");
	m_StartButton.setActionTodo([&]() {
		spdlog::info("Switch to GameState: Starting the game");
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
	m_QuitButton.setPosition({ m_RenderWindow.getSize().x / 2 - m_StartButton.getSize().x / 2 , 420 });
	m_QuitButton.setString("quit game");
	m_QuitButton.setActionTodo([&]() {
		spdlog::info("Quit game :(");
		m_RenderWindow.close();
	});

	//m_GameTitle.setPosition(350, 10);

	m_VolumeSlider.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	m_VolumeSlider.setString("volume");
	m_VolumeSlider.setPosition({100, 300});
	m_VolumeSlider.setSize({ 300, 30 });
	m_VolumeSlider.setOnProgressChange([&](float progress) {
		spdlog::info("Progress changed.");
		m_BackgroundMusic->setVolume(progress * 100);
	});
	m_VolumeSlider.setProgress(0.f);
	
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
	m_VolumeSlider.update(dt);
}

void MenuState::draw() const
{
	m_RenderWindow.clear();
	m_RenderWindow.draw(m_BackgroundSprite);
	m_RenderWindow.draw(m_GameTitle);
	m_RenderWindow.draw(m_StartButton);
	m_RenderWindow.draw(m_SettingsButton);
	m_RenderWindow.draw(m_QuitButton);
	m_RenderWindow.draw(m_VolumeSlider);
	m_RenderWindow.display();
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
	m_VolumeSlider.handleEvent(event);
}

void MenuState::onResume()
{
	m_BackgroundMusic->play();
}

void MenuState::onPause()
{
	m_BackgroundMusic->pause();
}

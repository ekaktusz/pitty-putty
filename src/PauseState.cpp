#include "PauseState.h"
#include "SettingsState.h"
#include <bagla-engine/asset-manager/AssetManager.h>
#include <bagla-engine/states/StateManager.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <SFML/Window/Event.hpp>

PauseState::PauseState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow) :
	bgl::State(stateManager, renderWindow),
																						  m_CountinueButton(renderWindow),
																						  m_SettingsButton(renderWindow),
																						  m_QuitButton(renderWindow),
																						  m_ReturnToMainButton(renderWindow)
{
	loadAssets();
	m_BackgroundTexture = bgl::AssetManager::getInstance().getTexture("menuBackground");
	m_BackgroundSprite.setTexture(m_BackgroundTexture);
	m_BackgroundMusic = &bgl::AssetManager::getInstance().getMusic("menuMusic");
	m_BackgroundMusic->play();
	m_GameTitle.setFont(bgl::AssetManager::getInstance().getFont("pixelFont"));
	m_GameTitle.setCharacterSize(100);
	m_GameTitle.setString("paused");
	m_GameTitle.setFillColor(sf::Color::White);
	m_GameTitle.setOutlineColor(sf::Color::Black);
	m_GameTitle.setOutlineThickness(5);

	sf::Vector2f center{ m_GameTitle.getGlobalBounds().width / 2.f, m_GameTitle.getGlobalBounds().height / 2.f };
	sf::Vector2f localBounds{ center.x + m_GameTitle.getLocalBounds().left, center.y + m_GameTitle.getLocalBounds().top };
	sf::Vector2f rounded{ std::round(localBounds.x), std::round(localBounds.y) };
	m_GameTitle.setOrigin(rounded);
	m_GameTitle.setPosition(m_RenderWindow.getSize().x / 2, 100);

	m_CountinueButton.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	m_CountinueButton.setSize({ 400, 50 });
	m_CountinueButton.setPosition({ m_RenderWindow.getSize().x / 2.f - m_CountinueButton.getSize().x / 2.f , 300.f});
	m_CountinueButton.setString("continue game");
	m_CountinueButton.setActionTodo([&]() {
		spdlog::info("Switch to GameState: Starting the game");
		m_StateManager.popState();
	});

	m_SettingsButton.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	m_SettingsButton.setSize({ 400, 50 });
	m_SettingsButton.setPosition({ m_RenderWindow.getSize().x / 2.f - m_CountinueButton.getSize().x / 2.f , 360.f });
	m_SettingsButton.setString("settings");
	m_SettingsButton.setActionTodo([&]() {
		spdlog::debug("Settings button pressed!");
		std::unique_ptr<SettingsState> settingsState = std::make_unique<SettingsState>(m_StateManager, m_RenderWindow);
		m_StateManager.pushState(std::move(settingsState));
	});

	m_ReturnToMainButton.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	m_ReturnToMainButton.setSize({ 400, 50 });
	m_ReturnToMainButton.setPosition({ m_RenderWindow.getSize().x / 2.f - m_CountinueButton.getSize().x / 2.f , 420.f });
	m_ReturnToMainButton.setString("return to main menu");
	m_ReturnToMainButton.setActionTodo([&]() {
		spdlog::info("Switch to MenuState: Starting the game");
		m_StateManager.resetToFirstState();
	});

	m_QuitButton.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	m_QuitButton.setSize({ 400, 50 });
	m_QuitButton.setPosition({ m_RenderWindow.getSize().x / 2 - m_CountinueButton.getSize().x / 2 , 480 });
	m_QuitButton.setString("quit game");
	m_QuitButton.setActionTodo([&]() {
		spdlog::info("Quit game :(");
		m_RenderWindow.close();
	});

	m_RenderWindow.setView(sf::View(static_cast<sf::Vector2f>(m_RenderWindow.getSize() / 2u), static_cast<sf::Vector2f>(m_RenderWindow.getSize())));
}

PauseState::~PauseState()
{
	m_BackgroundMusic->pause();
}

void PauseState::loadAssets()
{
	// For know it's empty, as MenuState already loaded everything we need
}

void PauseState::update(const sf::Time& dt)
{
	m_CountinueButton.update(dt);
	m_SettingsButton.update(dt);
	m_QuitButton.update(dt);
	m_ReturnToMainButton.update(dt);
}

void PauseState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.clear();
	target.draw(m_BackgroundSprite);
	target.draw(m_GameTitle);
	target.draw(m_CountinueButton);
	target.draw(m_SettingsButton);
	target.draw(m_QuitButton);
	target.draw(m_ReturnToMainButton);
}

void PauseState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		m_RenderWindow.close();
	}
	else if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			m_StateManager.popState();
		}
	}
	m_CountinueButton.handleEvent(event);
	m_SettingsButton.handleEvent(event);
	m_QuitButton.handleEvent(event);
	m_ReturnToMainButton.handleEvent(event);
}

void PauseState::onResume()
{
	if (m_BackgroundMusic->getStatus() != sf::Music::Status::Playing) 
	{
		m_BackgroundMusic->play();
	}
}

void PauseState::onPause()
{
	//m_BackgroundMusic->pause();
}

#include "MenuState.h"
#include "GameState.h"
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
	_startButton(renderWindow),
	_settingsButton(renderWindow),
	_quitButton(renderWindow)
{
	loadAssets();
	_backgroundTexture = bgl::AssetManager::getInstance().getTexture("menuBackground");
	_backgroundSprite.setTexture(_backgroundTexture);
	_backgroundMusic = &bgl::AssetManager::getInstance().getMusic("menuMusic");
	_backgroundMusic->setVolume(50);
	_backgroundMusic->setLoop(true);
	_backgroundMusic->play();
	_gameTitle.setFont(bgl::AssetManager::getInstance().getFont("pixelFont"));
	_gameTitle.setCharacterSize(100);
	_gameTitle.setString("Pitty Putty");
	_gameTitle.setFillColor(sf::Color::White);
	_gameTitle.setOutlineColor(sf::Color::Black);
	_gameTitle.setOutlineThickness(5);

	sf::Vector2f center { _gameTitle.getGlobalBounds().width / 2.f, _gameTitle.getGlobalBounds().height / 2.f };
	sf::Vector2f localBounds { center.x + _gameTitle.getLocalBounds().left, center.y + _gameTitle.getLocalBounds().top };
	sf::Vector2f rounded { std::round(localBounds.x), std::round(localBounds.y) };
	_gameTitle.setOrigin(rounded);
	_gameTitle.setPosition(_renderWindow.getSize().x / 2.f, 100.f);

	_startButton.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	_startButton.setSize({ 400, 50 });
	_startButton.setPosition({ _renderWindow.getSize().x / 2.f - _startButton.getSize().x / 2.f, 300.f });
	_startButton.setString("start game");
	_startButton.setCornerRadius(10.f);
	_startButton.setActionTodo([&]() {
		SPDLOG_INFO("Switch to GameState: Starting the game");
		_backgroundMusic->pause();
		std::unique_ptr<GameState> gameState = std::make_unique<GameState>(_stateManager, _renderWindow);
		_stateManager.pushState(std::move(gameState));
	});

	_settingsButton.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	_settingsButton.setSize({ 400, 50 });
	_settingsButton.setPosition({ _renderWindow.getSize().x / 2 - _startButton.getSize().x / 2, 360 });
	_settingsButton.setString("settings");
	_settingsButton.setActionTodo([&]() {
		SPDLOG_DEBUG("settings button pressed.");
		std::unique_ptr<SettingsState> settingsState = std::make_unique<SettingsState>(_stateManager, _renderWindow);
		_stateManager.pushState(std::move(settingsState));
	});

	_quitButton.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	_quitButton.setSize({ 400, 50 });
	_quitButton.setPosition({ _renderWindow.getSize().x / 2.f - _quitButton.getSize().x / 2.f, 420.f });
	_quitButton.setString("quit game");
	_quitButton.setActionTodo([&]() {
		SPDLOG_INFO("Quit game :(");
		_renderWindow.close();
	});

	// _gameTitle.setPosition(350, 10);

	SPDLOG_INFO("sx" + std::to_string(_renderWindow.getSize().x) + " sy:" + std::to_string(_renderWindow.getSize().y));
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
	_startButton.update(dt);
	_settingsButton.update(dt);
	_quitButton.update(dt);
}

void MenuState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.clear();
	target.draw(_backgroundSprite);
	target.draw(_gameTitle);
	target.draw(_startButton);
	target.draw(_settingsButton);
	target.draw(_quitButton);
}

void MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		_renderWindow.close();
	}
	_startButton.handleEvent(event);
	_settingsButton.handleEvent(event);
	_quitButton.handleEvent(event);
}

void MenuState::onResume()
{
	if (_backgroundMusic->getStatus() != sf::Music::Status::Playing)
	{
		_backgroundMusic->play();
	}
}

void MenuState::onPause() {}

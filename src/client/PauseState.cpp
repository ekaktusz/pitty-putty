#include "PauseState.h"
#include "SettingsState.h"
#include <SFML/Window/Event.hpp>
#include <bagla-engine/asset-manager/AssetManager.h>
#include <bagla-engine/states/StateManager.h>
#include <memory>
#include <spdlog/spdlog.h>

PauseState::PauseState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow) :
	bgl::State(stateManager, renderWindow),
	_countinueButton(renderWindow),
	_settingsButton(renderWindow),
	_quitButton(renderWindow),
	_returnToMainButton(renderWindow)
{
	loadAssets();
	_backgroundTexture = bgl::AssetManager::getInstance().getTexture("menuBackground");
	_backgroundSprite.setTexture(_backgroundTexture);
	_backgroundMusic = &bgl::AssetManager::getInstance().getMusic("menuMusic");
	_backgroundMusic->play();
	_gameTitle.setFont(bgl::AssetManager::getInstance().getFont("pixelFont"));
	_gameTitle.setCharacterSize(100);
	_gameTitle.setString("paused");
	_gameTitle.setFillColor(sf::Color::White);
	_gameTitle.setOutlineColor(sf::Color::Black);
	_gameTitle.setOutlineThickness(5);

	sf::Vector2f center { _gameTitle.getGlobalBounds().width / 2.f, _gameTitle.getGlobalBounds().height / 2.f };
	sf::Vector2f localBounds { center.x + _gameTitle.getLocalBounds().left, center.y + _gameTitle.getLocalBounds().top };
	sf::Vector2f rounded { std::round(localBounds.x), std::round(localBounds.y) };
	_gameTitle.setOrigin(rounded);
	_gameTitle.setPosition(_renderWindow.getSize().x / 2, 100);

	_countinueButton.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	_countinueButton.setSize({ 400, 50 });
	_countinueButton.setPosition({ _renderWindow.getSize().x / 2.f - _countinueButton.getSize().x / 2.f, 300.f });
	_countinueButton.setString("continue game");
	_countinueButton.setActionTodo([&]() {
		SPDLOG_INFO("Switch to GameState: Starting the game");
		_stateManager.popState();
	});

	_settingsButton.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	_settingsButton.setSize({ 400, 50 });
	_settingsButton.setPosition({ _renderWindow.getSize().x / 2.f - _countinueButton.getSize().x / 2.f, 360.f });
	_settingsButton.setString("settings");
	_settingsButton.setActionTodo([&]() {
		SPDLOG_DEBUG("Settings button pressed!");
		std::unique_ptr<SettingsState> settingsState = std::make_unique<SettingsState>(_stateManager, _renderWindow);
		_stateManager.pushState(std::move(settingsState));
	});

	_returnToMainButton.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	_returnToMainButton.setSize({ 400, 50 });
	_returnToMainButton.setPosition({ _renderWindow.getSize().x / 2.f - _countinueButton.getSize().x / 2.f, 420.f });
	_returnToMainButton.setString("return to main menu");
	_returnToMainButton.setActionTodo([&]() {
		SPDLOG_INFO("Switch to MenuState: Starting the game");
		_stateManager.resetToFirstState();
	});

	_quitButton.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	_quitButton.setSize({ 400, 50 });
	_quitButton.setPosition({ _renderWindow.getSize().x / 2 - _countinueButton.getSize().x / 2, 480 });
	_quitButton.setString("quit game");
	_quitButton.setActionTodo([&]() {
		SPDLOG_INFO("Quit game :(");
		_renderWindow.close();
	});

	_renderWindow.setView(sf::View(static_cast<sf::Vector2f>(_renderWindow.getSize() / 2u), static_cast<sf::Vector2f>(_renderWindow.getSize())));
}

PauseState::~PauseState()
{
	_backgroundMusic->pause();
}

void PauseState::loadAssets()
{
	// For know it's empty, as MenuState already loaded everything we need
}

void PauseState::update(const sf::Time& dt)
{
	_countinueButton.update(dt);
	_settingsButton.update(dt);
	_quitButton.update(dt);
	_returnToMainButton.update(dt);
}

void PauseState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.clear();
	target.draw(_backgroundSprite);
	target.draw(_gameTitle);
	target.draw(_countinueButton);
	target.draw(_settingsButton);
	target.draw(_quitButton);
	target.draw(_returnToMainButton);
}

void PauseState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		_renderWindow.close();
	}
	else if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			_stateManager.popState();
		}
	}
	_countinueButton.handleEvent(event);
	_settingsButton.handleEvent(event);
	_quitButton.handleEvent(event);
	_returnToMainButton.handleEvent(event);
}

void PauseState::onResume()
{
	if (_backgroundMusic->getStatus() != sf::Music::Status::Playing)
	{
		_backgroundMusic->play();
	}
}

void PauseState::onPause()
{
	//_backgroundMusic->pause();
}

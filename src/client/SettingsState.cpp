#include "SettingsState.h"

#include <bagla-engine/asset-manager/AssetManager.h>
#include <bagla-engine/states/State.h>
#include <bagla-engine/states/StateManager.h>

#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <spdlog/spdlog.h>

#include <cmath>

SettingsState::SettingsState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow) :
	bgl::State(stateManager, renderWindow),
	_volumeSlider(renderWindow),
	_backButton(renderWindow),
	_title()
{
	SPDLOG_INFO("state created");
	bgl::AssetManager::getInstance().loadTexture("../../assets/background/bg_settings.png", "settingsBackground");
	_backgroundTexture = bgl::AssetManager::getInstance().getTexture("settingsBackground");
	_backgroundSprite.setTexture(_backgroundTexture);

	// Background music is already loaded from menu state
	_backgroundMusic = &bgl::AssetManager::getInstance().getMusic("menuMusic");

	_volumeSlider.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	_volumeSlider.setString("volume");
	_volumeSlider.setSize({ 300, 30 });
	_volumeSlider.setPosition({ renderWindow.getSize().x / 2.f - _volumeSlider.getSize().x / 2.f, 360.f });
	_volumeSlider.setOnProgressChange([&](float progress) {
		SPDLOG_INFO("Progress changed.");
		_backgroundMusic->setVolume(progress * 100);
	});
	_volumeSlider.setProgress(_backgroundMusic->getVolume() / 100.f);

	_backButton.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	_backButton.setSize({ 50, 50 });
	_backButton.setPosition({ 30, 30 });
	_backButton.setString("<");
	_backButton.setActionTodo([&]() { stateManager.popState(); });

	_title.setFont(bgl::AssetManager::getInstance().getFont("pixelFont"));
	_title.setCharacterSize(100);
	_title.setString("settings");
	_title.setFillColor(sf::Color::White);
	_title.setOutlineColor(sf::Color::Black);
	_title.setOutlineThickness(5);

	sf::Vector2f center { _title.getGlobalBounds().width / 2.f, _title.getGlobalBounds().height / 2.f };
	sf::Vector2f localBounds { center.x + _title.getLocalBounds().left, center.y + _title.getLocalBounds().top };
	sf::Vector2f rounded { std::round(localBounds.x), std::round(localBounds.y) };
	_title.setOrigin(rounded);
	_title.setPosition(renderWindow.getSize().x / 2.f, 100.f);
}

void SettingsState::update(const sf::Time& dt)
{
	_volumeSlider.update(dt);
	_backButton.update(dt);
}

void SettingsState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.clear();
	target.draw(_backgroundSprite);
	target.draw(_volumeSlider);
	target.draw(_backButton);
	target.draw(_title);
}

void SettingsState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		_renderWindow.close();
	}
	_volumeSlider.handleEvent(event);
	_backButton.handleEvent(event);
}

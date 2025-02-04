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
	m_VolumeSlider(renderWindow),
	m_BackButton(renderWindow),
	m_Title()
{
	SPDLOG_INFO("state created");
	bgl::AssetManager::getInstance().loadTexture("../../assets/background/bg_settings.png", "settingsBackground");
	m_BackgroundTexture = bgl::AssetManager::getInstance().getTexture("settingsBackground");
	m_BackgroundSprite.setTexture(m_BackgroundTexture);

	// Background music is already loaded from menu state
	m_BackgroundMusic = &bgl::AssetManager::getInstance().getMusic("menuMusic");

	m_VolumeSlider.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	m_VolumeSlider.setString("volume");
	m_VolumeSlider.setSize({ 300, 30 });
	m_VolumeSlider.setPosition({ m_RenderWindow.getSize().x / 2.f - m_VolumeSlider.getSize().x / 2.f, 360.f });
	m_VolumeSlider.setOnProgressChange([&](float progress) {
		SPDLOG_INFO("Progress changed.");
		m_BackgroundMusic->setVolume(progress * 100);
	});
	m_VolumeSlider.setProgress(m_BackgroundMusic->getVolume() / 100.f);

	m_BackButton.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	m_BackButton.setSize({ 50, 50 });
	m_BackButton.setPosition({ 30, 30 });
	m_BackButton.setString("<");
	m_BackButton.setActionTodo([&]() { m_StateManager.popState(); });

	m_Title.setFont(bgl::AssetManager::getInstance().getFont("pixelFont"));
	m_Title.setCharacterSize(100);
	m_Title.setString("settings");
	m_Title.setFillColor(sf::Color::White);
	m_Title.setOutlineColor(sf::Color::Black);
	m_Title.setOutlineThickness(5);

	sf::Vector2f center { m_Title.getGlobalBounds().width / 2.f, m_Title.getGlobalBounds().height / 2.f };
	sf::Vector2f localBounds { center.x + m_Title.getLocalBounds().left, center.y + m_Title.getLocalBounds().top };
	sf::Vector2f rounded { std::round(localBounds.x), std::round(localBounds.y) };
	m_Title.setOrigin(rounded);
	m_Title.setPosition(m_RenderWindow.getSize().x / 2.f, 100.f);
}

void SettingsState::update(const sf::Time& dt)
{
	m_VolumeSlider.update(dt);
	m_BackButton.update(dt);
}

void SettingsState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.clear();
	target.draw(m_BackgroundSprite);
	target.draw(m_VolumeSlider);
	target.draw(m_BackButton);
	target.draw(m_Title);
}

void SettingsState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		m_RenderWindow.close();
	}
	m_VolumeSlider.handleEvent(event);
	m_BackButton.handleEvent(event);
}

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <bagla-engine/gui/Button.h>
#include <bagla-engine/gui/Slider.h>
#include <bagla-engine/states/State.h>

namespace bgl
{
class StateManager;
}

namespace sf
{
class Music;
}

class SettingsState : public bgl::State
{
public:
	SettingsState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow);
	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleEvent(const sf::Event& event) override;

private:
	sf::Music* m_BackgroundMusic;

	bgl::Slider m_VolumeSlider;

	bgl::Button m_BackButton;

	sf::Texture m_BackgroundTexture;
	sf::Sprite m_BackgroundSprite;

	sf::Text m_Title;
};
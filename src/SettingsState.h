#pragma once

#include <bagla-engine/states/State.h>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

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
	void draw() const override;
	void handleEvent(const sf::Event& event) override;

private:
	sf::Music* m_BackgroundMusic;

	sf::Texture m_BackgroundTexture;
	sf::Sprite m_BackgroundSprite;
};
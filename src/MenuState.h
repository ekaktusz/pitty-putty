#pragma once

#include <bagla-engine/states/State.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Text.hpp>

namespace bgl
{
	class StateManager;
}

class MenuState : public bgl::State
{
public:
	MenuState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow);
	void update(const sf::Time& dt) override;
	void draw() const override;
	void handleEvent(const sf::Event& event) override;

private:
	void loadAssets();
	sf::Texture m_BackgroundTexture;
	sf::Sprite m_BackgroundSprite;
	sf::Text m_GameTitle;

	sf::Music* m_BackgroundMusic;
};
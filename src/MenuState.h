#pragma once

#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <bagla-engine/gui/Button.h>
#include <bagla-engine/gui/Slider.h>
#include <bagla-engine/states/State.h>

namespace bgl
{
class StateManager;
}

class MenuState : public bgl::State
{
public:
	MenuState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow);
	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleEvent(const sf::Event& event) override;

	void onResume() override;
	void onPause() override;

private:
	void loadAssets();

private:
	sf::Texture m_BackgroundTexture;
	sf::Sprite m_BackgroundSprite;
	sf::Text m_GameTitle;

	bgl::Button m_StartButton;
	bgl::Button m_SettingsButton;
	bgl::Button m_QuitButton;

	sf::Music* m_BackgroundMusic;
};
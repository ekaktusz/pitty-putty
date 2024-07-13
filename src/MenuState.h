#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <bagla-engine/states/State.h>
#include <bagla-engine/gui/Button.h>
#include <bagla-engine/gui/CheckBox.h>
#include <bagla-engine/gui/Slider.h>
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

	sf::RectangleShape m_TransitionBackground;
	sf::Clock m_TransitionClock;
	sf::Time m_TransitionDuration = sf::seconds(2.f);
};

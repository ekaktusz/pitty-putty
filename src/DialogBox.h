#pragma once

#include <bagla-engine/GameObject.h>
#include <bagla-engine/graphics/RoundedRectangleShape.h>

#include <string>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

namespace bgl
{
class Camera;
}

class DialogBox : public sf::Drawable, public bgl::GameObject
{
public:
	DialogBox();

	void setSize(sf::Vector2f size);
	void setPosition(sf::Vector2f position);

	void setDialogString(const std::string& dialogString);
	void attachCamera(const bgl::Camera& camera);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(const sf::Time& dt) override;

	void start();

private:
	bool m_started = false;

	bgl::RoundedRectangleShape m_backgroundBox;

	sf::Vector2f m_Position;

	sf::Text m_dialogText;
	std::string m_dialogString;

	const bgl::Camera* m_attachedCamera;

	const sf::Time DURATION = sf::seconds(3.f);
	sf::Clock m_clock;

	std::string m_displayedText;
	float m_currentLineWidth = 0.f;

	const float X_MARGIN = 5.f;
	const float Y_MARGIN = 5.f;
};
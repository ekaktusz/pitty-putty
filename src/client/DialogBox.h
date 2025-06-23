#pragma once

#include <bagla-engine/GameObject.h>
#include <bagla-engine/graphics/RoundedRectangleShape.h>

#include <string>

#include <SFML/Graphics/CircleShape.hpp>
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
	bool _started = false;

	bgl::RoundedRectangleShape _backgroundBox;

	sf::Vector2f _position;

	sf::Text _dialogText;
	std::string _dialogString;

	const bgl::Camera* _attachedCamera;

	const sf::Time DURATION = sf::seconds(3.f);
	sf::Clock _clock;

	std::string _displayedText;
	float _currentLineWidth = 0.f;

	static constexpr float X_MARGIN = 5.f;
	static constexpr float Y_MARGIN = 5.f;
};
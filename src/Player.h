#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <bagla-engine/GameObject.h>

class Player : public bgl::GameObject
{
public:
	Player();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(const sf::Time& dt) override;
	void handleEvent(const sf::Event& event) override;
private:
	sf::RectangleShape m_RectangleShape;
};
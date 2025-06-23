#pragma once

#include <array>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

struct StarLayer
{
	sf::VertexArray vertices;
	sf::Vector2f velocity;
	float size;
};

class StarBackground : public sf::Drawable
{
public:
	StarBackground() = default;

	explicit StarBackground(sf::Vector2f size);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(const sf::Time& dt);

	void setPosition(sf::Vector2f position);

private:
	void initializeLayers();
	void updateLayer(StarLayer& layer, const sf::Time& dt);

private:
	static constexpr size_t LAYER_COUNT = 5;
	static constexpr size_t STARS_PER_LAYER = 1000;

	sf::Vector2f _position;
	sf::Vector2f _size;

	std::array<StarLayer, LAYER_COUNT> _layers;
};
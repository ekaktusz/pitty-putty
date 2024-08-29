#pragma once
#include <vector>
#include <array>
#include <bagla-engine/MathExtensions.h>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Drawable.hpp> 1329
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

class StarBackground : public sf::Drawable
{
public:
	StarBackground() = default;

	explicit StarBackground(sf::Vector2f size);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(const sf::Time& dt);

	void setPlayerVelocity(sf::Vector2f velocity);
	void setPosition(sf::Vector2f position);

private:
	static constexpr size_t LAYER_COUNT = 5;
	static constexpr size_t STARS_PER_LAYER = 1000;
	
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	sf::Vector2f m_playerVelocity;

	struct StarLayer
	{
		sf::VertexArray vertices;
		sf::Vector2f defaultSpeed;
		float size;
	};

	std::array<StarLayer, LAYER_COUNT> m_layers;

	void initializeLayers();
	void updateLayer(StarLayer& layer, const sf::Time& dt);
};
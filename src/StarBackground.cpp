#include "StarBackground.h"
#include "effolkronium/random.hpp"

StarBackground::StarBackground(sf::Vector2f size) : m_size(size)
{
	initializeLayers();
}

void StarBackground::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& layer : m_layers)
	{
		sf::VertexArray offsetVertices = layer.vertices;
		for (size_t i = 0; i < offsetVertices.getVertexCount(); ++i)
		{
			offsetVertices[i].position += m_position;
		}
		target.draw(offsetVertices, states);
	}
}

void StarBackground::update(const sf::Time& dt)
{
	for (auto& layer : m_layers)
	{
		updateLayer(layer, dt);
	}
}

void StarBackground::setPosition(sf::Vector2f position)
{
	m_position = position;
}

static sf::Color getRandomStarColor()
{
	const std::array<sf::Color, 4> colorOptions = { sf::Color::White, sf::Color::Yellow, sf::Color::Cyan, sf::Color::Magenta };
	const size_t colorChoice = effolkronium::random_static::get(0, 3);
	return colorOptions[colorChoice];
}

void StarBackground::initializeLayers()
{
	for (size_t i = 0; i < LAYER_COUNT; ++i)
	{
		m_layers[i].vertices.setPrimitiveType(sf::Points);
		m_layers[i].vertices.resize(STARS_PER_LAYER);
		m_layers[i].velocity.x = 5.0f + 5.0f * i; // Default speed for each layer
		m_layers[i].velocity.y = 5.0f + 5.0f * i; // Default speed for each layer
		m_layers[i].size = 1.0f + 0.5f * i;		  // Adjust size for each layer

		for (size_t j = 0; j < STARS_PER_LAYER; ++j)
		{
			const float x = effolkronium::random_static::get(0.f, m_size.x);
			const float y = effolkronium::random_static::get(0.f, m_size.y);

			m_layers[i].vertices[j].position = sf::Vector2f(x, y);
			m_layers[i].vertices[j].color = getRandomStarColor();
		}
	}
}

void StarBackground::updateLayer(StarLayer& layer, const sf::Time& dt)
{
	for (size_t i = 0; i < STARS_PER_LAYER; ++i)
	{
		sf::Vector2f& pos = layer.vertices[i].position;
		pos += layer.velocity * dt.asSeconds();

		// Wrap stars around the screen
		if (pos.x < 0 - m_position.x)
			pos.x = m_size.x;
		if (pos.x > m_size.x)
			pos.x = 0;
		if (pos.y < 0)
			pos.y = m_size.y;
		if (pos.y > m_size.y)
			pos.y = 0;
	}
}

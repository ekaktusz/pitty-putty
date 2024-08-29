#include "StarBackground.h"

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

void StarBackground::setPlayerVelocity(sf::Vector2f velocity)
{
	m_playerVelocity = velocity;
	m_playerVelocity.y *= 0;
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

void StarBackground::initializeLayers()
{
	for (size_t i = 0; i < LAYER_COUNT; ++i)
	{
		m_layers[i].vertices.setPrimitiveType(sf::Points);
		m_layers[i].vertices.resize(STARS_PER_LAYER);
		m_layers[i].defaultSpeed.x = 5.0f + 5.0f * i;  // Default speed for each layer
		m_layers[i].defaultSpeed.y = 5.0f + 5.0f * i;  // Default speed for each layer
		m_layers[i].size = 1.0f + 0.5f * i;   // Adjust size for each layer

		for (size_t j = 0; j < STARS_PER_LAYER; ++j)
		{
			float x = (float)bgl::randomInt(0, m_size.x);
			float y = (float)bgl::randomInt(0, m_size.y);

			m_layers[i].vertices[j].position = sf::Vector2f(x, y);
			m_layers[i].vertices[j].color = getRandomStarColor();
		}
	}
}

static sf::Color getRandomStarColor()
{
	int colorChoice = bgl::randomInt(0, 4);
	sf::Uint8 brightness = bgl::randomInt(150, 255);

	switch (colorChoice)
	{
	case 0: return sf::Color(brightness, brightness, brightness); // White
	case 1: return sf::Color(brightness, brightness, bgl::randomInt(200, 255)); // Light Blue
	case 2: return sf::Color(brightness, bgl::randomInt(200, 255), bgl::randomInt(150, 200)); // Light Orange
	case 3: return sf::Color(bgl::randomInt(200, 255), brightness, bgl::randomInt(200, 255)); // Light Purple
	default: return sf::Color(brightness, bgl::randomInt(200, 255), brightness); // Light Green
	}
}

void StarBackground::updateLayer(StarLayer& layer, const sf::Time& dt)
{
	sf::Vector2f totalVelocity = m_playerVelocity - layer.defaultSpeed;

	for (size_t i = 0; i < STARS_PER_LAYER; ++i)
	{
		sf::Vector2f& pos = layer.vertices[i].position;
		pos += totalVelocity * dt.asSeconds();

		// Wrap stars around the screen
		if (pos.x < 0 - m_position.x) pos.x = m_size.x;
		if (pos.x > m_size.x) pos.x = 0;
		if (pos.y < 0) pos.y = m_size.y;
		if (pos.y > m_size.y) pos.y = 0;
	}
}



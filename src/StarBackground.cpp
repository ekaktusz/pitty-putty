#include "StarBackground.h"
#include "effolkronium/random.hpp"

StarBackground::StarBackground(sf::Vector2f size) : _size(size)
{
	initializeLayers();
}

void StarBackground::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& layer : _layers)
	{
		sf::VertexArray offsetVertices = layer.vertices;
		for (size_t i = 0; i < offsetVertices.getVertexCount(); ++i)
		{
			offsetVertices[i].position += _position;
		}
		target.draw(offsetVertices, states);
	}
}

void StarBackground::update(const sf::Time& dt)
{
	for (auto& layer : _layers)
	{
		updateLayer(layer, dt);
	}
}

void StarBackground::setPosition(sf::Vector2f position)
{
	_position = position;
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
		_layers[i].vertices.setPrimitiveType(sf::Points);
		_layers[i].vertices.resize(STARS_PER_LAYER);
		_layers[i].velocity.x = 5.0f + 5.0f * i; // Default speed for each layer
		_layers[i].velocity.y = 5.0f + 5.0f * i; // Default speed for each layer
		_layers[i].size = 1.0f + 0.5f * i;		 // Adjust size for each layer

		for (size_t j = 0; j < STARS_PER_LAYER; ++j)
		{
			const float x = effolkronium::random_static::get(0.f, _size.x);
			const float y = effolkronium::random_static::get(0.f, _size.y);

			_layers[i].vertices[j].position = sf::Vector2f(x, y);
			_layers[i].vertices[j].color = getRandomStarColor();
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
		if (pos.x < 0 - _position.x)
			pos.x = _size.x;
		if (pos.x > _size.x)
			pos.x = 0;
		if (pos.y < 0)
			pos.y = _size.y;
		if (pos.y > _size.y)
			pos.y = 0;
	}
}

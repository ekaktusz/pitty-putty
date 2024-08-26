#pragma once
#include "Game.h"
#include <vector>
#include <array>
#include <bagla-engine/MathExtensions.h>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

class StarBackground : public sf::Drawable
{
public:
	StarBackground()
	{
		initializeLayers();
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		for (const auto& layer : m_layers)
		{
			target.draw(layer.vertices, states);
		}
	}

	void setVelocity(sf::Vector2f velocity)
	{
		m_velocity = velocity;
	}

	void update(const sf::Time& dt)
	{
		for (auto& layer : m_layers)
		{
			updateLayer(layer, dt);
		}
	}

	void setPosition(sf::Vector2f position)
	{
		m_position = position;
	}

private:
	static constexpr size_t LAYER_COUNT = 3;
	static constexpr size_t STARS_PER_LAYER = 500;

	sf::Vector2f m_position;

	struct StarLayer
	{
		sf::VertexArray vertices;
		float speed;
		float size;
	};

	std::array<StarLayer, LAYER_COUNT> m_layers;
	sf::Vector2f m_velocity;

	void initializeLayers()
	{
		for (size_t i = 0; i < LAYER_COUNT; ++i)
		{
			m_layers[i].vertices.setPrimitiveType(sf::Points);
			m_layers[i].vertices.resize(STARS_PER_LAYER);
			m_layers[i].speed = 0.1f + 0.2f * i;  // Adjust speed for each layer
			m_layers[i].size = 1.0f + 0.5f * i;   // Adjust size for each layer

			for (size_t j = 0; j < STARS_PER_LAYER; ++j)
			{
				float x = (float) bgl::randomInt(0, Game::WINDOW_WIDTH);
				float y = (float) bgl::randomInt(0, Game::WINDOW_HEIGHT);
				sf::Uint8 brightness = bgl::randomInt(100, 255);

				m_layers[i].vertices[j].position = sf::Vector2f(x, y);
				m_layers[i].vertices[j].color = sf::Color(brightness, brightness, brightness);
			}
		}
	}

	void updateLayer(StarLayer& layer, const sf::Time& dt)
	{
		for (size_t i = 0; i < STARS_PER_LAYER; ++i)
		{
			sf::Vector2f& pos = layer.vertices[i].position;
			pos += m_velocity * layer.speed * dt.asSeconds();

			// Wrap stars around the screen
			if (pos.x < 0) pos.x = Game::WINDOW_WIDTH;
			if (pos.x > Game::WINDOW_WIDTH) pos.x = 0;
			if (pos.y < 0) pos.y = Game::WINDOW_HEIGHT;
			if (pos.y > Game::WINDOW_HEIGHT) pos.y = 0;
		}
	}
};
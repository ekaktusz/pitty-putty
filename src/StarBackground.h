#pragma once
#include "Game.h"
#include <vector>
#include <array>
#include <bagla-engine/MathExtensions.h>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Drawable.hpp> 1329
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include "PlanetGraphics.h"

class StarBackground : public sf::Drawable
{
public:
	StarBackground() = default;

	explicit StarBackground(sf::Vector2f size) : m_size(size)
	{
		initializeLayers();
		m_planetGraphics.setPosition({ 150.f, 150.f });
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
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

		target.draw(m_planetGraphics);
	}

	void setPlayerVelocity(sf::Vector2f velocity)
	{
		m_playerVelocity = velocity;
		m_playerVelocity.y *= 0;
	}

	void update(const sf::Time& dt)
	{
		for (auto& layer : m_layers)
		{
			updateLayer(layer, dt);
		}

		m_planetGraphics.update(dt);
	}

	void setPosition(sf::Vector2f position)
	{
		m_position = position;
	}


private:
	static constexpr size_t LAYER_COUNT = 5;
	static constexpr size_t STARS_PER_LAYER = 1000;
	sf::Vector2f m_position;
	sf::Vector2f m_size{ Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT };
	sf::Vector2f m_playerVelocity;

	PlanetGraphics m_planetGraphics;

	struct StarLayer
	{
		sf::VertexArray vertices;
		sf::Vector2f defaultSpeed;
		float size;
	};

	std::array<StarLayer, LAYER_COUNT> m_layers;

	sf::Color getRandomStarColor() const
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

	void initializeLayers()
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

	void updateLayer(StarLayer& layer, const sf::Time& dt)
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
};
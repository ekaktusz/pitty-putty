#include "DialogBox.h"

#include <bagla-engine/camera/Camera.h>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <bagla-engine/asset-manager/AssetManager.h>

#include <spdlog/spdlog.h>

#include <algorithm>

DialogBox::DialogBox()
{
	m_backgroundBox.setFillColor(sf::Color::Black);
	m_backgroundBox.setOutlineColor(sf::Color::White);
	m_backgroundBox.setOutlineThickness(4.f);

	m_dialogText.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	m_dialogText.setFillColor(sf::Color::White);
	
}

void DialogBox::setSize(sf::Vector2f size)
{
	m_backgroundBox.setSize(size);
}

void DialogBox::setPosition(sf::Vector2f position)
{
	m_Position = position;
}

void DialogBox::setDialogString(const std::string& dialogString)
{
	m_dialogString = dialogString;
}

void DialogBox::attachCamera(const bgl::Camera& camera)
{
	m_attachedCamera = &camera;
}

void DialogBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_backgroundBox);
	target.draw(m_dialogText);
}

void DialogBox::update(const sf::Time& dt)
{
	m_backgroundBox.setPosition(m_attachedCamera->getPosition() + m_Position);
	m_dialogText.setPosition(m_attachedCamera->getPosition() + m_Position + sf::Vector2f { X_MARGIN, Y_MARGIN });

	if (m_started)
	{
		const float currentProgress = m_clock.getElapsedTime().asSeconds() / DURATION.asSeconds();
		const size_t currentPosition = std::min((size_t) std::round(currentProgress * m_dialogString.size()), m_dialogString.size());

		if (currentPosition > m_displayedText.size())
		{
			sf::String newChar = m_dialogString[m_displayedText.size()];
			float newCharWidth = m_dialogText.getFont()->getGlyph(newChar[0], m_dialogText.getCharacterSize(), false).advance;

			if (m_currentLineWidth + newCharWidth > m_backgroundBox.getSize().x - X_MARGIN)
			{
				m_displayedText += '\n';
				m_currentLineWidth = 0;
			}

			m_displayedText += newChar;
			m_currentLineWidth += newCharWidth;
		}
		else
		{
			m_started = false;
			// TODO: dialog over
		}

		m_dialogText.setString(m_displayedText);

	}
}

void DialogBox::start()
{
	m_started = true;
	m_clock.restart();
}

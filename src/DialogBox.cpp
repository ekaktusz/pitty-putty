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
	m_dialogText.setPosition(m_attachedCamera->getPosition() + m_Position + sf::Vector2f { 10.f, 10.f });

	if (m_started)
	{
		const float currentProgress = m_clock.getElapsedTime().asSeconds() / DURATION.asSeconds();
		const size_t currentPosition = std::round(currentProgress * m_dialogString.size());
		const int currentTextPosition = std::clamp(currentPosition, (size_t) 0, m_dialogString.size());
		sf::String currentDialogString = m_dialogText.getString();

		spdlog::info(" size: " + std::to_string(m_dialogString.size()) + " currentpos:" + std::to_string(currentTextPosition - 1));
		currentDialogString += m_dialogString[currentTextPosition - 1];
		m_dialogText.setString(currentDialogString);
		
		const float currentTextLengthPixel = m_dialogText.getGlobalBounds().left + m_dialogText.getGlobalBounds().width;
		const float maxTextLength = m_backgroundBox.getGlobalBounds().left + m_backgroundBox.getGlobalBounds().width;

		if (currentTextLengthPixel > maxTextLength + 10)
		{
			
		}
	}
}

void DialogBox::start()
{
	m_started = true;
	m_clock.restart();
}

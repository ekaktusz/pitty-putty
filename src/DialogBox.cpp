#include "DialogBox.h"

#include <bagla-engine/camera/Camera.h>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <bagla-engine/asset-manager/AssetManager.h>

#include <spdlog/spdlog.h>

#include <algorithm>

DialogBox::DialogBox()
{
	_backgroundBox.setFillColor(sf::Color::Black);
	_backgroundBox.setOutlineColor(sf::Color::White);
	_backgroundBox.setOutlineThickness(4.f);

	_dialogText.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));
	_dialogText.setFillColor(sf::Color::White);
}

void DialogBox::setSize(sf::Vector2f size)
{
	_backgroundBox.setSize(size);
}

void DialogBox::setPosition(sf::Vector2f position)
{
	_position = position;
}

void DialogBox::setDialogString(const std::string& dialogString)
{
	_dialogString = dialogString;
}

void DialogBox::attachCamera(const bgl::Camera& camera)
{
	_attachedCamera = &camera;
}

void DialogBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_backgroundBox);
	target.draw(_dialogText);
}

void DialogBox::update(const sf::Time& dt)
{
	_backgroundBox.setPosition(_attachedCamera->getPosition() + _position);
	_dialogText.setPosition(_attachedCamera->getPosition() + _position + sf::Vector2f { X_MARGIN, Y_MARGIN });

	if (_started)
	{
		const float currentProgress = _clock.getElapsedTime().asSeconds() / DURATION.asSeconds();
		const size_t currentPosition = std::min((size_t)std::round(currentProgress * _dialogString.size()), _dialogString.size());

		if (currentPosition > _displayedText.size())
		{
			const sf::String newChar = _dialogString[_displayedText.size()];
			const float newCharWidth = _dialogText.getFont()->getGlyph(newChar[0], _dialogText.getCharacterSize(), false).advance;

			if (_currentLineWidth + newCharWidth > _backgroundBox.getSize().x - X_MARGIN)
			{
				_displayedText += '\n';
				_currentLineWidth = 0;
			}
			SPDLOG_INFO(_displayedText);
			_displayedText += newChar;
			_currentLineWidth += newCharWidth;
		}
		else
		{
			_started = false;
			// TODO: dialog over
		}

		_dialogText.setString(_displayedText);
	}
}

void DialogBox::start()
{
	_started = true;
	_clock.restart();
}

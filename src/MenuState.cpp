#include "MenuState.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

void MenuState::update(const sf::Time& dt)
{

}

void MenuState::draw() const
{
	m_RenderWindow.clear();
	m_RenderWindow.display();
}


void MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		m_RenderWindow.close();
	}
}
#include "GameState.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <bagla-engine/asset-manager/AssetManager.h>
#include <bagla-engine/map/Map.h>
#include <bagla-engine/map/TileLayer.h>
#include <bagla-engine/states/StateManager.h>
#include "PauseState.h"
#include <bagla-engine/physics/PhysicsWorld.h>
#include "Game.h"
#include "tmxlite/ObjectTypes.hpp"
#include <spdlog/spdlog.h>
#include "BulletManager.h"

GameState::GameState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow) : 
	bgl::State(stateManager, renderWindow),
	m_Camera(renderWindow),
	m_Map(nullptr),
	m_PhysicsWorld(bgl::PhysicsWorld::getInstance())
{
	loadAssets();
	//m_PhysicsWorld.initDebugDraw(renderWindow);
}

GameState::~GameState()
{
	
}

void GameState::update(const sf::Time& dt)
{
	m_Player1.update(dt);
	const sf::Vector2f offset = m_Player1.getCenterPosition() - m_Camera.getCenterPosition();
	m_Camera.move(offset * dt.asSeconds() * 10.f);
	m_PhysicsWorld.update(dt);
	BulletManager::getInstance().update(dt);

	m_starBackground.setPlayerVelocity( (- 0.1f) * m_Player1.getVelocity());
	m_starBackground.setPosition(m_Camera.getPosition());
	m_starBackground.update(dt);
	//m_Camera.update(dt);
}

void GameState::draw(sf::RenderTarget &target, sf::RenderStates states)const
{
	target.clear();
	target.draw(m_PhysicsWorld);
	target.draw(m_Player1);
	target.draw(m_Map->getTileLayer("backlayer"));
	target.draw(m_starBackground);
	target.draw(BulletManager::getInstance());
}

void GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		m_RenderWindow.close();
	}
	else if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			std::unique_ptr<PauseState> pauseState = std::make_unique<PauseState>(m_StateManager, m_RenderWindow);
			m_StateManager.pushState(std::move(pauseState));
		}
	}
	m_Player1.handleEvent(event);
	BulletManager::getInstance().handleEvent(event);
}

void GameState::onResume()
{

}

void GameState::onPause()
{

}

void GameState::onStart()
{
	m_Map = &bgl::AssetManager::getInstance().getMap("testmap");
	

	m_Camera.setWorldBoundaries(0, 0, m_Map->getSize().x, m_Map->getSize().y);
	m_Camera.attach(m_RenderWindow);

	m_Player1.setPosition(getPlayerStartingPosition());
}

void GameState::loadAssets()
{
	bgl::AssetManager::getInstance().loadMap("../../assets/maps/testmap.tmx", "testmap");
}

sf::Vector2f GameState::getPlayerStartingPosition() const
{
	const bgl::ObjectLayer& playerStartingPositionLayer = m_Map->getObjectLayer("starting_pos");
	
	const tmx::Object& object = playerStartingPositionLayer.getFirstObject();

	return { object.getPosition().x, object.getPosition().y };
}
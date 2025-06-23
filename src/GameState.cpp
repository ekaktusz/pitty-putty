#include "GameState.h"

#include "BulletManager.h"
#include "PauseState.h"
#include "tmxlite/ObjectTypes.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <bagla-engine/asset-manager/AssetManager.h>
#include <bagla-engine/map/Map.h>
#include <bagla-engine/map/TileLayer.h>
#include <bagla-engine/physics/PhysicsWorld.h>
#include <bagla-engine/states/StateManager.h>

#include <spdlog/spdlog.h>

GameState::GameState(bgl::StateManager& stateManager, sf::RenderWindow& renderWindow) :
	bgl::State(stateManager, renderWindow),
	_camera(renderWindow),
	_map(nullptr),
	_physicsWorld(bgl::PhysicsWorld::getInstance())
{
	loadAssets();
	//_physicsWorld.initDebugDraw(renderWindow);

	_fpsCounter.setString("buttonString");
	_fpsCounter.setFont(bgl::AssetManager::getInstance().getFont("upheaval"));

	//_fpsCounter.setFont(;
}

GameState::~GameState()
{
	bgl::AssetManager::getInstance().unloadMap("testmap");
	_physicsWorld.cleanUp();
}

void GameState::update(const sf::Time& dt)
{
	_player1.update(dt);
	const sf::Vector2f offset = _player1.getCenterPosition() - _camera.getCenterPosition();
	_camera.move(offset * dt.asSeconds() * 10.f);
	_physicsWorld.update(dt);
	BulletManager::getInstance().update(dt);

	_starBackground.update(dt);

	_dialogBoxTest.update(dt);

	_fpsCounter.setPosition(sf::Vector2f(20.f, 20.f) + _camera.getPosition());
	_fpsCounter.setString(std::to_string(1.f / dt.asSeconds()));
	//_camera.update(dt);
}

void GameState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.clear();
	target.draw(_starBackground);
	target.draw(_physicsWorld);
	target.draw(_player1);
	target.draw(_map->getTileLayer("backlayer"));
	target.draw(BulletManager::getInstance());
	target.draw(_fpsCounter);
	target.draw(_dialogBoxTest);
}

void GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		_renderWindow.close();
	}
	else if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			std::unique_ptr<PauseState> pauseState = std::make_unique<PauseState>(_stateManager, _renderWindow);
			_stateManager.pushState(std::move(pauseState));
		}
	}
	_player1.handleEvent(event);
}

void GameState::onResume() {}

void GameState::onPause() {}

void GameState::onStart()
{
	_map = &bgl::AssetManager::getInstance().getMap("testmap");

	_camera.setWorldBoundaries(0, 0, _map->getSize().x, _map->getSize().y);
	_camera.attach(_renderWindow);

	_starBackground = StarBackground(_map->getSize());

	_player1.setPosition(getPlayerStartingPosition());

	_dialogBoxTest.setSize({ 500.f, 200.f });
	_dialogBoxTest.setDialogString(
		"Important: When you use mismatched microphone and external speaker devices, it might cause an echo. Important: When you use mismatched microphone and external speaker devices, it might cause an echo!");
	_dialogBoxTest.attachCamera(_camera);
	_dialogBoxTest.setPosition({ 50.f, 500.f });
	_dialogBoxTest.start();
}

void GameState::loadAssets()
{
	bgl::AssetManager::getInstance().loadMap("../../assets/maps/testmap.tmx", "testmap");
}

sf::Vector2f GameState::getPlayerStartingPosition() const
{
	const bgl::ObjectLayer& playerStartingPositionLayer = _map->getObjectLayer("starting_pos");

	const tmx::Object& object = playerStartingPositionLayer.getFirstObject();

	return { object.getPosition().x, object.getPosition().y };
}
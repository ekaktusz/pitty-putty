#pragma once

#include <bagla-engine/animation/AnimationComponent.h>
#include <bagla-engine/animation/Animation.h>
#include <bagla-engine/asset-manager/AssetManager.h>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>

class PlanetGraphics : public sf::Drawable
{
public:
	PlanetGraphics() 
	{
		bgl::AssetManager::getInstance().loadTexture("../../assets/spritesheets/planets/earth.png", "earth-texture");
		const sf::Texture& planetAnimationTexture = bgl::AssetManager::getInstance().getTexture("earth-texture");
		auto spinningAnimation = std::make_unique<bgl::Animation>(planetAnimationTexture, sf::Vector2i(80, 80), sf::Vector2i(0, 0), sf::Vector2i(19, 0), sf::seconds(0.05f));
		m_animationComponent.addAnimation("idle", std::move(spinningAnimation));
		m_animationComponent.setCurrentAnimation("idle");
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(m_animationComponent);
	}

	void update(const sf::Time& dt) 
	{
		m_animationComponent.update(dt);
	}

	void setPosition(sf::Vector2f position) 
	{
		m_animationComponent.setPosition(position);
	}

private:
	bgl::AnimationComponent m_animationComponent;
};
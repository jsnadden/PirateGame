#pragma once
#include <string>
#include "SDL.h"
#include "Components.hpp"
#include "TextureManager.hpp"

class ColliderComponent : public Component
{
public:
	SDL_Rect collider;
	std::string tag;

	SDL_Texture* colliderTexture;
	SDL_Rect srcRect, destRect;

	TransformComponent* transform;

	ColliderComponent(std::string tagIn)
	{
		tag = tagIn;
	}

	ColliderComponent(std::string tagIn, int xPos, int yPos, int size)
	{
		tag = tagIn;
		
		collider.x = xPos;
		collider.y = yPos;
		collider.w = collider.h = size;
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		colliderTexture = TextureManager::loadTexture("assets/collider.png");
		srcRect = { 0, 0, 32, 32 };
		destRect = { collider.x, collider.y, collider.w, collider.h };
	}

	void update(float dt) override
	{
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.w = (transform->width) * (transform->scale);
			collider.h = (transform->height) * (transform->scale);
		}

		destRect.x = collider.x - Game::camera.x;
		destRect.y = collider.y - Game::camera.y;
	}

	void draw() override
	{
		// Comment out to hide collision boxes:
		//TextureManager::Draw(colliderTexture, srcRect, destRect, SDL_FLIP_NONE);
	}

};
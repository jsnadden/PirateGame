#pragma once
#include <string>
#include "SDL.h"
#include "Components.hpp"
#include "Graphics.hpp"

class ColliderComponent : public Component
{
private:

	Graphics* graphics;
	Camera* camera;

	SDL_Rect collider;
	std::string tag;

	SDL_Texture* colliderTexture;
	SDL_Rect srcRect, destRect;

	TransformComponent* transform;

public:

	ColliderComponent(std::string t)
	{
		tag = t;
	}

	ColliderComponent(std::string t, int xPos, int yPos, int size)
	{
		tag = t;
		
		collider.x = xPos;
		collider.y = yPos;
		collider.w = collider.h = size;
	}

	~ColliderComponent()
	{
		graphics = nullptr;
		camera = nullptr;
		colliderTexture = nullptr;
		transform = nullptr;
	}

	void init() override
	{
		graphics = Graphics::GetInstance();
		camera = Camera::GetInstance();

		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		colliderTexture = graphics->LoadTexture("assets/collider.png");
		srcRect = { 0, 0, 32, 32 };
		destRect = { collider.x, collider.y, collider.w, collider.h };
	}

	SDL_Rect Location()
	{
		return destRect;
	}

	void Update() override
	{
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->GetPosition()->x);
			collider.y = static_cast<int>(transform->GetPosition()->y);
			collider.w = (transform->GetWidth()) * (transform->GetScale());
			collider.h = (transform->GetHeight()) * (transform->GetScale());
		}

		destRect.x = collider.x - camera->OriginX();
		destRect.y = collider.y - camera->OriginY();
	}

	void draw() override
	{
		// Comment out to hide collision boxes:
		//graphics->DrawTexture(colliderTexture, &srcRect, &destRect, SDL_FLIP_NONE);
	}

};
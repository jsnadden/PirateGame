#pragma once
#include <string>
#include "SDL.h"
#include "Components.hpp"
#include "Managers.hpp"

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

	float relX = 0.0f;
	float relY = 0.0f;
	float relW = 1.0f;
	float relH = 1.0f;

	bool visible;

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
			entity->addComponent<TransformComponent>(collider.x, collider.y, collider.w, collider.h);
		}
		transform = &entity->getComponent<TransformComponent>();

		colliderTexture = graphics->LoadTexture("assets/collider.png");
		srcRect = { 0, 0, 32, 32 };
		destRect = { collider.x, collider.y, collider.w, collider.h };
	}

	void SetRelative(float x, float y, float w, float h)
	{
		relX = x;
		relY = y;
		relW = w;
		relH = h;
	}

	SDL_Rect Location()
	{
		return destRect;
	}

	Vector2D Centre()
	{
		return Vector2D(destRect.x + destRect.w / 2, destRect.y + destRect.h / 2);
	}

	TransformComponent* Transform()
	{
		return transform;
	}

	void Show()
	{
		visible = true;
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

		destRect.x = collider.x + (relX * collider.w) - camera->OriginX();
		destRect.y = collider.y + (relY * collider.w) - camera->OriginY();
		destRect.w = relW * collider.w;
		destRect.h = relH * collider.h;
	}

	void draw() override
	{
		if (visible)
		{
			graphics->DrawTexture(colliderTexture, &srcRect, &destRect, SDL_FLIP_NONE);
			visible = false;
		}
	}

};
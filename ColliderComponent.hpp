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

	DynRect collider;
	std::string tag;

	SDL_Texture* colliderTexture;
	SDL_Rect srcRect;

	TransformComponent* transform;

	float relX = 0.0f;
	float relY = 0.0f;
	float relW = 1.0f;
	float relH = 1.0f;

	bool visible;

public:

	ColliderComponent(std::string t, float xPos, float yPos, float size)
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

		colliderTexture = graphics->LoadTexture("assets/tiles/collider.png");
		srcRect = { 0, 0, 32, 32 };
	}

	void SetRelative(float x, float y, float w, float h)
	{
		relX = x;
		relY = y;
		relW = w;
		relH = h;
	}

	DynRect Rectangle()
	{
		return collider;
	}

	void SetRectangle(DynRect R)
	{
		collider = R;
	}

	TransformComponent* Transform()
	{
		return transform;
	}

	void Show()
	{
		visible = true;
	}

	void SyncToTransform()
	{
		collider.x = transform->GetPosition()->x + (relX * transform->GetWidth() * transform->GetScale());
		collider.y = transform->GetPosition()->y + (relY * transform->GetHeight() * transform->GetScale());
		collider.w = relW * transform->GetWidth() * transform->GetScale();
		collider.h = relH * transform->GetHeight() * transform->GetScale();
		collider.vx = transform->GetVelocity()->x;
		collider.vy = transform->GetVelocity()->y;
	}

	void Update() override
	{
		if (tag != "terrain")
		{
			SyncToTransform();
		}
	}

	void draw() override
	{
		if (visible)
		{
			SDL_Rect destRect = collider.SDLCast();
			destRect.x -= camera->OriginX();
			destRect.y -= camera->OriginY();

			graphics->DrawTexture(colliderTexture, &srcRect, &destRect, SDL_FLIP_NONE);
			visible = false;
		}
	}

};
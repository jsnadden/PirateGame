#pragma once
#include "Components.hpp"
#include "SDL.h"
#include "TextureManager.hpp"
#include "Animation.hpp"
#include <map>
#include "AssetManager.hpp"

class SpriteComponent : public Component
{
private:

	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frame = 0;
	int speed = 100;

public:

	int animIndex = 0;
	std::map<const char*, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
		setTexture(id);
	}

	SpriteComponent(std::string id, bool isAnimated)
	{
		setTexture(id);
		animated = isAnimated;

		// TODO: systemise these specifications, as they are only 
		//		 compatible with the one sprite sheet
		Animation idle = Animation(0, 3, 100);
		Animation sail = Animation(1, 3, 100);

		animations.emplace("Idle", idle);
		animations.emplace("Sail", sail);

		Play("Idle");
	}

	~SpriteComponent()
	{}

	void setTexture(std::string id)
	{
		texture = Game::assets->GetTexture(id);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update(float dt) override
	{
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frame);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;

		destRect.w = (transform->width) * (transform->scale);
		destRect.h = (transform->height) * (transform->scale);
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	void Play(const char* animName)
	{
		animIndex = animations[animName].index;
		frame = animations[animName].frame;
		speed = animations[animName].speed;
	}


};


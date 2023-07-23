#pragma once
#include <map>
#include "SDL.h"
#include "Components.hpp"
#include "Graphics.hpp"
#include "Assets.hpp"
#include "Timer.hpp"
#include "Animation.hpp"

class SpriteComponent : public Component
{
public:

	static enum WRAP_MODE { once = 0, loop = 1 };

private:

	Graphics* graphics;
	Assets* assets;
	Timer* timer;

	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	bool animated = false;
	std::map<std::string, Animation*> animations;

	bool animationComplete = true;
	WRAP_MODE mode = once;
	int totalFrames;
	int row;
	float elapsedTime = 0.0f;
	float timePerFrame;
	float endTime;

public:

	SpriteComponent() = default;
	
	SpriteComponent(std::string path, bool isAnimated = false, WRAP_MODE m = loop)
	{
		graphics = Graphics::GetInstance();
		assets = Assets::GetInstance();
		timer = Timer::GetInstance();

		setTexture(path);

		animated = isAnimated;
		animationComplete = !isAnimated;
		mode = m;
			
	}

	~SpriteComponent()
	{
		graphics = nullptr;
		timer = nullptr;
		transform = nullptr;
		texture = nullptr;

		animations.clear();
	}

	void setTexture(std::string path)
	{
		texture = assets->GetTexture(path);
	}

	void Flip(SDL_RendererFlip flip)
	{
		spriteFlip = flip;
	}

	void AddAnimation(std::string id, Animation* anim)
	{
		animations.emplace(id, anim);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = 0;
		srcRect.w = transform->GetWidth();
		srcRect.h = transform->GetHeight();
	}

	void update() override
	{
		if (!animationComplete)
		{
			elapsedTime += timer->DeltaTime();

			if (elapsedTime > endTime)
			{
				if (mode == loop)
				{
					elapsedTime = 0.0f;
				}
				else if (mode == once)
				{
					animationComplete = true;
					elapsedTime = endTime - timePerFrame;
				}
			}

			srcRect.x = srcRect.w * static_cast<int>(elapsedTime / timePerFrame);
			srcRect.y = row * srcRect.h;
		}

		// TODO subtract camera coordinates below
		destRect.x = static_cast<int>(transform->GetPosition().x);
		destRect.y = static_cast<int>(transform->GetPosition().y);

		destRect.w = static_cast<int>(transform->GetWidth() * transform->GetScale());
		destRect.h = static_cast<int>(transform->GetHeight() * transform->GetScale());
	}

	void draw() override
	{
		graphics->DrawTexture(texture, &srcRect, &destRect, transform->GetRotation(), spriteFlip);
	}

	void Play(std::string animName)
	{
		row = animations[animName]->row;
		totalFrames = animations[animName]->nFrames;

		timePerFrame = 1.0f / animations[animName]->speed;
		endTime = totalFrames * timePerFrame;
	}


};


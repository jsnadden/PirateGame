#pragma once
#include <map>
#include "SDL.h"
#include "Components.hpp"
#include "Managers.hpp"
#include "Animation.hpp"

class SpriteComponent : public Component
{
public:

	static enum WRAP_MODE { once = 0, loop = 1 };

private:

	Graphics* graphics;
	Assets* assets;
	Timer* timer;
	Camera* camera;

	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	bool visible;

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
		camera = Camera::GetInstance();

		setTexture(path);

		visible = true;

		animated = isAnimated;
		animationComplete = !isAnimated;
		mode = m;
			
	}

	~SpriteComponent()
	{
		graphics = nullptr;
		assets = nullptr;
		timer = nullptr;
		camera = nullptr;

		transform = nullptr;
		texture = nullptr;

		for (auto& a : animations)
		{
			if (a.second != nullptr)
			{
				delete a.second;
			}
		}
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

	bool Visible()
	{
		return visible;
	}

	void SetVisibility(bool vis)
	{
		visible = vis;
	}

	void AddAnimation(std::string id, int r, int f, float s)
	{
		animations[id] = new Animation(r, f, s);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = 0;
		srcRect.w = transform->GetWidth();
		srcRect.h = transform->GetHeight();
	}

	void Update() override
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

			int frame = static_cast<int>(elapsedTime / timePerFrame);
			if (frame >= totalFrames)
			{
				frame = totalFrames - 1;
			}

			srcRect.x = srcRect.w * frame;
			srcRect.y = row * srcRect.h;
		}
		
		destRect.x = static_cast<int>(transform->GetPosition()->x) - camera->OriginX();
		destRect.y = static_cast<int>(transform->GetPosition()->y) - camera->OriginY();

		destRect.w = static_cast<int>(transform->GetWidth() * transform->GetScale());
		destRect.h = static_cast<int>(transform->GetHeight() * transform->GetScale());
	}

	void draw() override
	{
		if (visible)
		{
			graphics->DrawTexture(texture, &srcRect, &destRect, transform->GetRotation(), spriteFlip);
		}
	}

	void Play(std::string animName)
	{
		row = animations[animName]->row;
		totalFrames = animations[animName]->nFrames;

		timePerFrame = 1.0f / animations[animName]->speed;
		endTime = totalFrames * timePerFrame;
	}


};


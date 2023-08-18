#pragma once
#include "src/Structs/Animation.hpp"

struct SpriteComponent
{
public:

	static enum WRAP_MODE { once = 0, loop = 1 };

	SDL_Texture* texture = nullptr;
	SDL_Rect srcRect;
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	std::map<std::string, Animation*> animations;

	bool visible = true;

	bool animated = false;

	bool animationComplete = true;
	WRAP_MODE mode = once;
	int totalFrames;
	float elapsedTime = 0.0f;
	float timePerFrame;
	float endTime;

	SpriteComponent(std::string path, int spriteWidth, int spriteHeight, bool isAnimated = false, WRAP_MODE m = loop)
	{
		texture = Assets::GetInstance()->GetTexture(path);

		visible = true;

		animated = isAnimated;
		animationComplete = !isAnimated;
		mode = m;

		srcRect.x = srcRect.y = 0;
		srcRect.w = spriteWidth;
		srcRect.h = spriteHeight;
	}

	~SpriteComponent()
	{
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

	void Play(std::string animName)
	{
		if (animations.find(animName) != animations.end())
		{
			srcRect.y = srcRect.h * animations[animName]->row;
			totalFrames = animations[animName]->nFrames;

			timePerFrame = 1.0f / animations[animName]->speed;
			endTime = totalFrames * timePerFrame;
		}
	}

};
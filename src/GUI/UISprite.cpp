#include "src/GUI/UISprite.hpp"

UISprite::UISprite(Vector2D centre, std::string imgPath, int scale)
{
	clipped = false;
	animated = false;

	spriteSheet = assets->GetTexture(imgPath);

	SDL_QueryTexture(spriteSheet, NULL, NULL, &destRect.w, &destRect.h);
	destRect.w *= scale;
	destRect.h *= scale;
	destRect.x = static_cast<int>(centre.x) - destRect.w / 2;
	destRect.y = static_cast<int>(centre.y) - destRect.h / 2;
}

UISprite::UISprite(Vector2D centre, std::string imgPath, int scale, int w, int h, bool anim, int nCells, float frameTime)
{
	timer = Timer::GetInstance();

	clipped = true;
	animated = anim;

	spriteSheet = assets->GetTexture(imgPath);

	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = w;
	srcRect.h = h;

	destRect.w = scale * w;
	destRect.h = scale * h;
	destRect.x = static_cast<int>(centre.x) - destRect.w / 2;
	destRect.y = static_cast<int>(centre.y) - destRect.h / 2;

	numCells = nCells;
	elapsedTime = 0.0f;
	timePerFrame = frameTime;
	endTime = nCells * frameTime;
}

UISprite::~UISprite()
{
	graphics = nullptr;
	assets = nullptr;
	input = nullptr;
	timer = nullptr;

	spriteSheet = nullptr;
}

void UISprite::Update()
{
	if (animated)
	{
		elapsedTime += timer->DeltaTime();

		if (elapsedTime > endTime)
		{
			elapsedTime = 0.0f;
		}

		int frame = static_cast<int>(elapsedTime / timePerFrame);
		if (frame >= numCells)
		{
			frame = numCells - 1;
		}

		srcRect.x = srcRect.w * frame;
	}
}

void UISprite::Draw()
{
	graphics->DrawTexture(spriteSheet, clipped ? &srcRect : NULL, &destRect, 0.0f, imgFlip);
}

void UISprite::Flip()
{
	imgFlip = imgFlip ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
}

void UISprite::SetCell(int index)
{
	if (index >= 0 || index < numCells)
		srcRect.x = index * srcRect.w;
}

#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Managers.hpp"


class UILabelComponent : public Component
{
private:

	Graphics* graphics;
	Assets* assets;

	SDL_Rect location;
	std::string textString;
	std::string font;
	SDL_Color fontColour;
	SDL_Texture* texture;
	int textSize;

public:

	UILabelComponent(int xpos, int ypos, std::string text, std::string fontPath, int size, SDL_Color& colour)
	{
		graphics = Graphics::GetInstance();
		assets = Assets::GetInstance();

		location.x = xpos;
		location.y = ypos;
		textString = text;
		font = fontPath;
		textSize = size;
		fontColour = colour;

		texture = assets->GetText(textString, font, textSize, fontColour);
		SDL_QueryTexture(texture, NULL, NULL, &location.w, &location.h);
	}

	~UILabelComponent()
	{
		graphics = nullptr;
		assets = nullptr;
		texture = nullptr;
	}

	void CentreV()
	{
		int height;
		SDL_QueryTexture(texture, NULL, NULL, NULL, &height);
		location.y = (Graphics::DEF_WINDOW_HEIGHT - height) / 2;
	}

	void CentreH()
	{
		int width;
		SDL_QueryTexture(texture, NULL, NULL, &width, NULL);
		location.x = (Graphics::DEF_WINDOW_WIDTH - width) / 2;
	}

	void draw() override
	{
		graphics->DrawTexture(texture, nullptr, &location);
	}

};
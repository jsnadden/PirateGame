#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "ECS.hpp"
#include "AssetManager.hpp"
#include "Game.hpp"


class UILabelComponent : public Component
{
public:

	UILabelComponent(int xpos, int ypos, std::string textIn, std::string fontIn, SDL_Color& colourIn)
	{
		position.x = xpos;
		position.y = ypos;
		text = textIn;
		font = fontIn;
		colour = colourIn;

		SetLabelText(textIn, fontIn);
	}

	~UILabelComponent()
	{}

	void SetLabelText(std::string textIn, std::string fontIn)
	{
		SDL_Surface* tempSurface = TTF_RenderText_Blended(Game::assets->GetFont(fontIn), textIn.c_str(), colour);
		texture = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
		SDL_FreeSurface(tempSurface);

		SDL_QueryTexture(texture, nullptr, nullptr, &position.w, &position.h);
	}

	void draw() override
	{
		SDL_RenderCopy(Game::renderer, texture, nullptr, &position);
	}


private:

	SDL_Rect position;
	std::string text;
	std::string font;
	SDL_Color colour;
	SDL_Texture* texture;

};
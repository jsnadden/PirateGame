#pragma once
#include <map>
#include <SDL_mixer.h>
#include "src/Managers/Graphics.hpp"

class Assets
{
public:

	static Assets* GetInstance();
	static void Release();

	SDL_Texture* GetTexture(std::string path);
	SDL_Texture* GetText(std::string text, std::string fontPath, int size, SDL_Color fillColour);
	SDL_Texture* GetOutlinedText(std::string text, std::string fontPath, int size, SDL_Color fillColour, int outlineSize, SDL_Color outlineColour);

	Mix_Music* GetMusic(std::string path);
	Mix_Chunk* GetSFX(std::string path);

private:

	Assets();
	~Assets();

	static Assets* instance;

	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, SDL_Texture*> texts;
	std::map<std::string, TTF_Font*> fonts;
	std::map<std::string, Mix_Music*> music;
	std::map<std::string, Mix_Chunk*> SFX;

	TTF_Font* GetFont(std::string path, int size);
};


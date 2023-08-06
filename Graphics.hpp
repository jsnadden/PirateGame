#pragma once
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

class Graphics
{
private:

	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_Color backgroundColour;

	SDL_Rect windowRect;

	static Graphics* instance;
	static bool initialised;

	Graphics();
	~Graphics();

	bool Init();

	bool fullscreen = false;
	bool minimised = false;
	bool mouseFocus = true;
	bool keyboardFocus = true;


public:

	// TODO: should read these in from a config
	static const int DEF_WINDOW_WIDTH = 1280;
	static const int DEF_WINDOW_HEIGHT = 720;

	static Graphics* GetInstance();
	static void Release();

	static bool HasInitialised();

	void ClearRenderer();
	void Render();

	SDL_Texture* LoadTexture(std::string path);
	void DrawTexture(SDL_Texture* tex, SDL_Rect* sRect = nullptr, SDL_Rect* dRect = nullptr, float rot = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);
	SDL_Texture* LoadText(std::string text, TTF_Font* font, SDL_Color fillColour);
	SDL_Texture* LoadOutlinedText(std::string text, TTF_Font* font, SDL_Color fillColour, int outlineSize, SDL_Color outlineColour = { 0,0,0,0 });

	void WindowTitle(const char* newTitle);
	void SetBackgroundColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	void DrawRectangle(SDL_Color colour, SDL_Rect* rect);
	void FillWindow(SDL_Color colour);

	SDL_Rect* ViewRect();
	int CentreX();
	int CentreY();

	void ToggleFullscreen();
	void SetFullScreen(bool fs);
	void HandleWindowEvent(SDL_Event& e);

	bool IsFullscreen();
	bool IsMinimised();
	bool HasMouseFocus();
	bool HasKeyboardFocus();


};


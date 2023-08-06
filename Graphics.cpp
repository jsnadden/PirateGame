#include "Graphics.hpp"

Graphics* Graphics::instance = nullptr;
bool Graphics::initialised = false;

Graphics::Graphics()
{
	renderer = nullptr;
	initialised = Init();

	backgroundColour.r = 0;
	backgroundColour.g = 0;
	backgroundColour.b = 0;
	backgroundColour.a = 0;
}

Graphics::~Graphics()
{
	SDL_DestroyWindow(window);
	window = nullptr;

	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

Graphics* Graphics::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Graphics();
	}

	return instance;
}

void Graphics::Release()
{
	delete instance;
	instance = nullptr;

	initialised = false;
}

bool Graphics::Init()
{
	// Start SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Create window, error check
	window = SDL_CreateWindow("-", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DEF_WINDOW_WIDTH, DEF_WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Create renderer, error check, set background to white
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// Start image library, error check
	int imgflags = IMG_INIT_PNG;
	if (!(IMG_Init(imgflags) & imgflags))
	{
		printf("Renderer could not be created! IMG_Error: %s\n", IMG_GetError());
		return false;
	}

	if (TTF_Init() < 0)
	{
		printf("SDL TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
		return false;
	}

	return true;
}

bool Graphics::HasInitialised()
{
	return initialised;
}

SDL_Texture* Graphics::LoadTexture(std::string path)
{
	SDL_Texture* tex = nullptr;


	SDL_Surface* tempSurface = IMG_Load(path.c_str());
	if (tempSurface == nullptr) {
		printf("Image %s could not be loaded! IMG_Error: %s\n", path.c_str(), IMG_GetError());
		return nullptr;
	}

	tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
	if (tex == nullptr) {
		printf("Text surface creation failed! SDL_Error: %s\n", SDL_GetError());
	}

	SDL_FreeSurface(tempSurface);

	return tex;
}

SDL_Texture* Graphics::LoadText(std::string text, TTF_Font* font, SDL_Color fillColour)
{
	SDL_Texture* tex = nullptr;

	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), fillColour);
	if (textSurface == nullptr) {
		printf("Text surface creation failed! TTF_Error: %s\n", TTF_GetError());
		return nullptr;
	}

	tex = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (tex == nullptr) {
		printf("Text texture creation failed! SDL_Error: %s\n", SDL_GetError());
	}

	SDL_FreeSurface(textSurface);

	return tex;
}

SDL_Texture* Graphics::LoadOutlinedText(std::string text, TTF_Font* font, SDL_Color fillColour, int outlineSize, SDL_Color outlineColour)
{
	SDL_Texture* tex = nullptr;

	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), fillColour);
	if (textSurface == nullptr) {
		printf("Text surface creation failed! TTF_Error: %s\n", TTF_GetError());
		return nullptr;
	}

	TTF_SetFontOutline(font, outlineSize);

	SDL_Surface* outlineSurface = TTF_RenderText_Blended(font, text.c_str(), outlineColour);
	if (outlineSurface == nullptr) {
		printf("Text outline surface creation failed! TTF_Error: %s\n", TTF_GetError());
		return nullptr;
	}

	TTF_SetFontOutline(font, 0);

	SDL_Rect rect = { outlineSize, outlineSize, textSurface->w, textSurface->h };
	SDL_SetSurfaceBlendMode(textSurface, SDL_BLENDMODE_BLEND);
	SDL_BlitSurface(textSurface, NULL, outlineSurface, &rect);

	tex = SDL_CreateTextureFromSurface(renderer, outlineSurface);
	if (tex == nullptr) {
		printf("Text texture creation failed! SDL_Error: %s\n", SDL_GetError());
	}

	SDL_FreeSurface(textSurface);
	SDL_FreeSurface(outlineSurface);

	return tex;
}

void Graphics::ClearRenderer()
{
	SDL_SetRenderDrawColor(renderer, backgroundColour.r, backgroundColour.g, backgroundColour.b, backgroundColour.a);
	SDL_RenderClear(renderer);
}

void Graphics::Render()
{
	SDL_RenderPresent(renderer);
}

void Graphics::DrawTexture(SDL_Texture* tex, SDL_Rect* sRect, SDL_Rect* dRect, float rot, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(renderer, tex, sRect, dRect, rot, NULL, flip);
}

void Graphics::WindowTitle(const char* newTitle)
{
	SDL_SetWindowTitle(window, newTitle);
}

void Graphics::SetBackgroundColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	backgroundColour = { r, g, b, a };
}

void Graphics::DrawRectangle(SDL_Color colour, SDL_Rect* rect)
{
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderFillRect(renderer, rect);
}

void Graphics::FillWindow(SDL_Color colour)
{
	DrawRectangle(colour, ViewRect());
}

SDL_Rect* Graphics::ViewRect()
{
	windowRect.x = windowRect.y = 0;
	SDL_GetWindowSize(window, &windowRect.w, &windowRect.h);
	return &windowRect;
}

void Graphics::ToggleFullscreen()
{
	Uint32 fullscreenFlag = SDL_WINDOW_FULLSCREEN;
	fullscreen = !(SDL_GetWindowFlags(window) & fullscreenFlag);
	SDL_SetWindowFullscreen(window, fullscreen ? fullscreenFlag : 0);
}

void Graphics::SetFullScreen(bool fs)
{
	Uint32 fullscreenFlag = SDL_WINDOW_FULLSCREEN;
	fullscreen = fs;
	SDL_SetWindowFullscreen(window, fullscreen ? fullscreenFlag : 0);
}

void Graphics::HandleWindowEvent(SDL_Event& e)
{
	switch (e.window.event)
	{
	case SDL_WINDOWEVENT_EXPOSED:
		SDL_RenderPresent(renderer);
		break;

	case SDL_WINDOWEVENT_ENTER:
		mouseFocus = true;
		break;

	case SDL_WINDOWEVENT_LEAVE:
		mouseFocus = false;
		break;

	case SDL_WINDOWEVENT_FOCUS_GAINED:
		keyboardFocus = true;
		break;

	case SDL_WINDOWEVENT_FOCUS_LOST:
		keyboardFocus = false;
		break;

	case SDL_WINDOWEVENT_MINIMIZED:
		minimised = true;
		break;

	case SDL_WINDOWEVENT_MAXIMIZED:
		minimised = false;
		break;

	case SDL_WINDOWEVENT_RESTORED:
		minimised = false;
		break;
	}
}

bool Graphics::IsFullscreen()
{
	return fullscreen;
}

bool Graphics::IsMinimised()
{
	return minimised;
}

bool Graphics::HasMouseFocus()
{
	return mouseFocus;
}

bool Graphics::HasKeyboardFocus()
{
	return keyboardFocus;
}
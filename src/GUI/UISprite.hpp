#pragma once
#include <SDL.h>
#include "src/GUI/UIElement.hpp"

class UISprite :
    public UIElement
{
private:

    Timer* timer;

    SDL_Texture* spriteSheet;
    SDL_Rect srcRect, destRect;
    SDL_RendererFlip imgFlip = SDL_FLIP_NONE;

    bool clipped = false;
    bool animated = false;

    int numCells = 1;
    float elapsedTime = 0.0f;
    float timePerFrame = 1.0f;
    float endTime = 1.0f;

public:

    // Whole image
    UISprite(Vector2D centre, std::string imgPath, int scale = 1);

    // Cell sheet, for animation or just clipping the top left corner
    UISprite(Vector2D centre, std::string imgPath, int scale, int w, int h, bool anim = false, int nCells = 1, float frameTime = 1.0f);

    ~UISprite();

    void Update();
    void Draw();

    void Flip();
    void SetCell(int index);

};


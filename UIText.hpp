#pragma once
#include "UIElement.hpp"

class UIText :
    public UIElement
{
private:

    SDL_Rect textRect;
    Vector2D centre;

    SDL_Texture* textTexture;
    std::string text;
    std::string fontPath = "assets/arcade_font.ttf";
    int fontSize;
    SDL_Color fontColour{ 0xff, 0xff, 0xff, 0xff };

public:

    UIText(Vector2D centre, std::string text, int size = 18);
    ~UIText();

    void Update() {}
    void Draw();

};


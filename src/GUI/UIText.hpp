#pragma once
#include "src/GUI/UIElement.hpp"

class UIText :
    public UIElement
{
private:

    SDL_Rect textRect;
    Vector2D centre;

    SDL_Texture* textTexture;
    std::string text;
    std::string fontPath = "assets/fonts/arcade.ttf";
    int fontSize;
    SDL_Color fontColour;

public:

    UIText(Vector2D centre, std::string text, int size = 18, SDL_Color colour = {0xff, 0xff, 0xff, 0xff});
    ~UIText();

    void Update();
    void Draw();

};


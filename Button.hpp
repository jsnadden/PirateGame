#pragma once
#include "UIElement.hpp"
#include "Collision.hpp"

class Button :
    public UIElement
{
private:

    SDL_Rect textRect;
    SDL_Rect buttonRect;
    Vector2D centre;

    SDL_Color defaultColour{0x00, 0x00, 0x00, 0xff};
    SDL_Color hoverColour{ 0x30, 0x30, 0x30, 0xff };
    SDL_Color downColour{ 0x60, 0x60, 0x60, 0xff };
    
    SDL_Texture* textTexture;
    std::string text;
    std::string fontPath = "assets/arcade_font.ttf";
    int fontSize = 18;
    SDL_Color fontColour{ 0xff, 0xff, 0xff, 0xff };

    bool hover;
    bool down;
    bool activated;

public:

    Button(Vector2D centre, std::string text);
    ~Button();

    void Update();
    void Draw();

    //TODO setters for member vars

    bool TestActivated();
};


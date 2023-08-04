#pragma once
#include "UIElement.hpp"
#include "Collision.hpp"

class Button :
    public UIElement
{
private:

    Vector2D centre;
    SDL_Rect buttonRect;
    int offset = 10;

    bool hover = false;
    bool down = false;
    bool activated = false;

    SDL_Color defaultButtonColour{0x00, 0x00, 0x00, 0xff};
    SDL_Color hoverButtonColour{ 0x60, 0x60, 0x60, 0xff };
    SDL_Color downButtonColour{ 0x30, 0x30, 0x30, 0xff };

    bool hasText = false;

    SDL_Rect textRect;

    std::string text = " ";
    std::string fontPath = "assets/arcade_font.ttf";
    int fontSize = 18;

    SDL_Color defaultTextColour{ 0xff, 0xff, 0xff, 0xff };
    SDL_Color hoverTextColour{ 0xff, 0xff, 0xff, 0xff };
    SDL_Color downTextColour{ 0xff, 0xff, 0xff, 0xff };
    
    SDL_Texture* defaultTextTexture = nullptr;
    SDL_Texture* hoverTextTexture = nullptr;
    SDL_Texture* downTextTexture = nullptr;

    bool hasImage = false;

    SDL_Rect srcRect;
    SDL_Texture* imageTexture = nullptr;
    SDL_RendererFlip imageFlip;

public:

    // Constructor for button with text
    Button(Vector2D position, std::string text);

    // Constructor for button with image
    Button(Vector2D position, int width, int height, int scale, std::string imgPath, SDL_RendererFlip flip = SDL_FLIP_NONE);

    ~Button();

    void Update();
    void Draw();

    bool IsDown();
    bool IsActivated();

    void CentreOn(Vector2D position);
    void SetButtonColours(SDL_Color defaultColour, SDL_Color hoverColour, SDL_Color downColour);

    void InitText();
    void SetText(std::string t);
    void SetFont(std::string path);
    void SetFontSize(int size);
    void SetTextColours(SDL_Color defaultColour, SDL_Color hoverColour, SDL_Color downColour);

    void SetImage(std::string path, int width, int height, SDL_RendererFlip flip = SDL_FLIP_NONE);

};


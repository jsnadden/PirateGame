#include "UIText.hpp"

UIText::UIText(Vector2D c, std::string t, int size)
    : UIElement()
{
    text = t;
    fontSize = size;

    textTexture = assets->GetText(text, fontPath, fontSize, fontColour);

    centre = c;
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.x = c.x - (textRect.w / 2);
    textRect.y = c.y - (textRect.h / 2);
}

UIText::~UIText()
{
    graphics = nullptr;
    assets = nullptr;
    input = nullptr;

    textTexture = nullptr;
}

void UIText::Draw()
{
    graphics->DrawTexture(textTexture, NULL, &textRect);
}



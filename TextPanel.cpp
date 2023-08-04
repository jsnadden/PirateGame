#include "TextPanel.hpp"

TextPanel::TextPanel(Vector2D position, std::string imgPath, int s)
{
    panelTexture = assets->GetTexture(imgPath);

    centre = position;
    scale = s;

    SDL_QueryTexture(panelTexture, NULL, NULL, &destRect.w, &destRect.h);
    destRect.w *= scale;
    destRect.h *= scale;
    destRect.x = static_cast<int>(position.x) - destRect.w / 2;
    destRect.y = static_cast<int>(position.y) - destRect.h / 2;

}

TextPanel::~TextPanel()
{
    graphics = nullptr;
    assets = nullptr;
    input = nullptr;

    panelTexture = nullptr;
}

void TextPanel::Update()
{}

void TextPanel::Draw()
{
    graphics->DrawTexture(panelTexture, NULL, &destRect);
}

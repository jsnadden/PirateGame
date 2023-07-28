#include "Button.hpp"

Button::Button(Vector2D c, std::string t)
	: UIElement()
{
	
    down = hover = false;

    text = t;

    textTexture = assets->GetText(text, fontPath, fontSize, fontColour);

    centre = c;
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.x = c.x - (textRect.w / 2);
    textRect.y = c.y - (textRect.h / 2);

    int offset = 10;
    buttonRect.w = textRect.w + offset;
    buttonRect.h = textRect.h + offset;
    buttonRect.x = c.x - (buttonRect.w / 2);
    buttonRect.y = c.y - (buttonRect.h / 2);
}

Button::~Button()
{
    graphics = nullptr;
    assets = nullptr;
    input = nullptr;

    textTexture = nullptr;
}

void Button::Update()
{
    SDL_Rect cursorLocation;
    cursorLocation.x = input->MousePosition().x;
    cursorLocation.y = input->MousePosition().y;
    cursorLocation.w = cursorLocation.h = 1;

    if (Collision::AABB(buttonRect, cursorLocation))
    {
        hover = true;

        if (input->MouseButtonPressed(Input::left))
        {
            down = true;
        }

        if (input->MouseButtonReleased(Input::left) && down)
        {
            activated = true;
        }
    }
    else
    {
        hover = false;
    }

    if (input->MouseButtonReleased(Input::left))
    {
        down = false;
    }
}

void Button::Draw()
{
    SDL_Color currentColour = defaultColour;
    if (hover)
    {
        currentColour = hoverColour;
    }
    if (down)
    {
        currentColour = downColour;
    }

    graphics->DrawRectangle(currentColour, &buttonRect);
    graphics->DrawTexture(textTexture, NULL, &textRect);
}

bool Button::TestActivated()
{
    bool act = activated;
    activated = false;
    return act;
}

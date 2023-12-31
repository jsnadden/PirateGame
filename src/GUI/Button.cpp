#include "src/GUI/Button.hpp"

Button::Button(Vector2D position, std::string t, int outline)
	: UIElement()
{
    centre = position; 
    
    down = hover = false;

    hasText = true;
    hasTextOutline = (outline > 0);
    outlineSize = outline;
    text = t;

    InitText();
}

Button::Button(Vector2D position, int width, int height, int scale, std::string imgPath, SDL_RendererFlip flip)
{
    centre = position; 
    
    down = hover = false;

    hasImage = true;
    SetImage(imgPath, width, height, flip);

    buttonRect.x = centre.x - (width * scale) / 2;
    buttonRect.y = centre.y - (height * scale) / 2;
    buttonRect.w = width * scale;
    buttonRect.h = height * scale;
}

Button::~Button()
{
    graphics = nullptr;
    assets = nullptr;
    input = nullptr;

    imageTexture = nullptr;

    defaultTextTexture = nullptr;
    hoverTextTexture = nullptr;
    downTextTexture = nullptr;
}

void Button::Update()
{
    int cursorX = input->MousePosition().x;
    int cursorY = input->MousePosition().y;

    if (Collision::PointRect(cursorX, cursorY, buttonRect))
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
    SDL_Color currentColour;
    SDL_Texture* currentText;

    if (hasImage)
    {
        srcRect.y = 0;
        if (active && hover)
        {
            srcRect.y = srcRect.h;
        }
        if (active && down)
        {
            srcRect.y = 2 * srcRect.h;
        }
        graphics->DrawTexture(imageTexture, &srcRect, &buttonRect, 0.0f, imageFlip);
    }
    else
    {
        currentColour = defaultButtonColour;
        if (active && hover)
        {
            currentColour = hoverButtonColour;
        }
        if (active && down)
        {
            currentColour = downButtonColour;
        }
        graphics->DrawRectangle(currentColour, &buttonRect);
    }

    if (hasText)
    {
        currentText = defaultTextTexture;
        if (active && hover)
        {
            currentText = hoverTextTexture;
        }
        if (active && down)
        {
            currentText = downTextTexture;
        }

        graphics->DrawTexture(currentText, NULL, &textRect);
        currentText = nullptr;
    }
         
    if (!active)
    {
        graphics->DrawRectangle({ 0, 0, 0, 125 }, &buttonRect);
    }
}

bool Button::IsDown()
{
    return down;
}

bool Button::IsActivated()
{
    bool act = activated;
    activated = false;
    return act;
}

void Button::CentreOn(Vector2D position)
{
    centre = position;
    if (hasText)
    {
        InitText();
    }
    else
    {
        buttonRect.x = centre.x - (buttonRect.w / 2);
        buttonRect.y = centre.y - (buttonRect.h / 2);
    }
}

void Button::SetButtonColours(SDL_Color defaultColour, SDL_Color hoverColour, SDL_Color downColour)
{
    defaultButtonColour = defaultColour;
    hoverButtonColour = hoverColour;
    downButtonColour = downColour;
}

void Button::InitText()
{
    if (hasTextOutline)
    {
        defaultTextTexture = assets->GetOutlinedText(text, fontPath, fontSize, defaultTextColour, outlineSize, {0x00, 0x00, 0x00, 0xff});
        hoverTextTexture = assets->GetOutlinedText(text, fontPath, fontSize, hoverTextColour, outlineSize, { 0x00, 0x00, 0x00, 0xff });
        downTextTexture = assets->GetOutlinedText(text, fontPath, fontSize, downTextColour, outlineSize, { 0x00, 0x00, 0x00, 0xff });
    }
    else
    {
        defaultTextTexture = assets->GetText(text, fontPath, fontSize, defaultTextColour);
        hoverTextTexture = assets->GetText(text, fontPath, fontSize, hoverTextColour);
        downTextTexture = assets->GetText(text, fontPath, fontSize, downTextColour);
    }

    SDL_QueryTexture(defaultTextTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.x = centre.x - (textRect.w / 2);
    textRect.y = centre.y - (textRect.h / 2);

    buttonRect.w = textRect.w + offset;
    buttonRect.h = textRect.h + offset;
    buttonRect.x = centre.x - (buttonRect.w / 2);
    buttonRect.y = centre.y - (buttonRect.h / 2);
}

void Button::SetText(std::string t)
{
    hasText = true;
    text = t;
    InitText();
}

void Button::SetFont(std::string path)
{
    fontPath = path;
    InitText();
}

void Button::SetFontSize(int size)
{
    fontSize = size;
    InitText();
}

void Button::SetTextColours(SDL_Color defaultColour, SDL_Color hoverColour, SDL_Color downColour)
{
    defaultTextColour = defaultColour;
    hoverTextColour = hoverColour;
    downTextColour = downColour;
    
    if (hasTextOutline)
    {
        defaultTextTexture = assets->GetOutlinedText(text, fontPath, fontSize, defaultTextColour, outlineSize, { 0x00, 0x00, 0x00, 0xff });
        hoverTextTexture = assets->GetOutlinedText(text, fontPath, fontSize, hoverTextColour, outlineSize, { 0x00, 0x00, 0x00, 0xff });
        downTextTexture = assets->GetOutlinedText(text, fontPath, fontSize, downTextColour, outlineSize, { 0x00, 0x00, 0x00, 0xff });
    }
    else
    {
        defaultTextTexture = assets->GetText(text, fontPath, fontSize, defaultTextColour);
        hoverTextTexture = assets->GetText(text, fontPath, fontSize, hoverTextColour);
        downTextTexture = assets->GetText(text, fontPath, fontSize, downTextColour);
    }
}

void Button::SetImage(std::string path, int width, int height, SDL_RendererFlip flip)
{
    srcRect = { 0, 0, width, height };
    imageTexture = assets->GetTexture(path);
    imageFlip = flip;
}

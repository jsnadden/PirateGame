#include "Picker.hpp"

Picker::Picker(Vector2D position, std::string t, int s)
{
    centre = position;
    scale = s;

    index = 0;

    Vector2D titleOffset(0, -32 * s);
    Vector2D buttonOffset(220 * s, 0);

    panel = new Panel(position, "assets/GUI/12longpanel.png", scale);
    rightButton = new Button(position + buttonOffset, 32, 32, scale, "assets/GUI/sidearrowbutton.png");
    leftButton = new Button(position - buttonOffset, 32, 32, scale, "assets/GUI/sidearrowbutton.png", SDL_FLIP_HORIZONTAL);
    title = new UIText(position + titleOffset, t, fontSize * scale, { 0xff, 0xff, 0xff, 0xff });
}

Picker::~Picker()
{
    graphics = nullptr;
    assets = nullptr;
    input = nullptr;

    delete panel;
    panel = nullptr;

    delete title;
    title = nullptr;

    delete leftButton;
    delete rightButton;
    leftButton = nullptr;
    rightButton = nullptr;

    for (auto& o : options)
    {
        delete o;
        o = nullptr;
    }
    options.clear();
}

void Picker::Update()
{
    rightButton->Update();
    leftButton->Update();

    if (rightButton->IsActivated() && !options.empty())
    {
        index++;
        if (index >= options.size()) index--;
    }

    if (leftButton->IsActivated() && !options.empty())
    {
        index--;
        if (index < 0) index++;
    }
}

void Picker::Draw()
{
    rightButton->Draw();
    leftButton->Draw();
    panel->Draw();
    title->Draw();

    if (!options.empty())
    {
        CurrentOption()->Draw();
    }
}

int Picker::Index()
{
    return index;
}

void Picker::SetIndex(int i)
{
    index = i;
}

bool Picker::HasChanged()
{
    return index != lastindex;
}

void Picker::Confirm()
{
    lastindex = index;
}

size_t Picker::OptionCount()
{
    return options.size();
}

UIText* Picker::CurrentOption()
{
    return options[index];
}

UIText* Picker::Option(size_t i)
{
    return (i >= 0 && i <= options.size()) ? options[i] : nullptr;
}

void Picker::AddOption(std::string name)
{
    options.emplace_back(new UIText(centre, name, fontSize * scale, {0x00, 0x00, 0x00, 0xff}));
}
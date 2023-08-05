#include "PauseMenu.hpp"

PauseMenu::PauseMenu()
{

    backPanel = new Panel(Vector2D(Graphics::SCREEN_WIDTH / 2, Graphics::SCREEN_HEIGHT / 2), "assets/GUI/pausepanel.png", 2);
    title = new UIText(Vector2D(400.0f, 200.0f), "PAUSED", 36);

    buttons["settings"] = new Button(Vector2D(400.0f, 350.0f), "SETTINGS");
    //buttons["settings"]->SetButtonColours(TRANSPARENT, TRANSPARENT, TRANSPARENT);
    //buttons["settings"]->SetTextColours(WHITE, LIGHT_GREY, BLACK);

    buttons["quit"] = new Button(Vector2D(400.0f, 400.0f), "QUIT TO MAIN MENU");
    //buttons["quit"]->SetButtonColours(TRANSPARENT, TRANSPARENT, TRANSPARENT);
    //buttons["quit"]->SetTextColours(WHITE, LIGHT_GREY, BLACK);

    buttons["endgame"] = new Button(Vector2D(400.0f, 450.0f), "QUIT TO DESKTOP");
    //buttons["endgame"]->SetButtonColours(TRANSPARENT, TRANSPARENT, TRANSPARENT);
    //buttons["endgame"]->SetTextColours(WHITE, LIGHT_GREY, BLACK);

}

PauseMenu::~PauseMenu()
{
    graphics = nullptr;
    assets = nullptr;
    input = nullptr;

    backPanel = nullptr;
    title = nullptr;
    
    for (auto& b : buttons)
    {
        delete b.second;
        b.second = nullptr;
    }
    buttons.clear();
}

void PauseMenu::Update()
{
    for (auto& b : buttons)
    {
        b.second->Update();
    }
}

void PauseMenu::Draw()
{
    backPanel->Draw();
    title->Draw();

    for (auto& b : buttons)
    {
        b.second->Draw();
    }
}

bool PauseMenu::Pressed(std::string buttonID)
{
    return buttons[buttonID]->IsActivated();
}

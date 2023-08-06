#include "PauseMenu.hpp"

PauseMenu::PauseMenu()
{
    SDL_Rect* view = graphics->ViewRect();
    int centreX = view->w / 2;
    int centreY = view->h / 2;
    backPanel = new Panel(Vector2D(centreX, centreY), "assets/GUI/pausepanel.png", 2);
    title = new UIText(Vector2D(centreX, centreY - 120), "PAUSED", 36);
    buttons["save"] = new Button(Vector2D(centreX, centreY -20), "SAVE GAME");
    buttons["settings"] = new Button(Vector2D(centreX, centreY + 30), "SETTINGS");
    buttons["quit"] = new Button(Vector2D(centreX, centreY + 80), "QUIT TO MAIN MENU");
    buttons["endgame"] = new Button(Vector2D(centreX, centreY + 130), "QUIT TO DESKTOP");

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

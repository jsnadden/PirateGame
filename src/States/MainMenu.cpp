#include "src/States/MainMenu.hpp"

MainMenu::MainMenu()
	: State()
{
	states = States::GetInstance();

	SDL_Rect* view = graphics->ViewRect();
	int centreX = view->w / 2;
	int centreY = view->h / 2;
	
	elements["logo_placeholder"] = new UIText(Vector2D(centreX, centreY - 150), "GAME LOGO HERE", 48);
	elements["start"] = new Button(Vector2D(centreX, centreY + 30), "NEW GAME");
	elements["load"] = new Button(Vector2D(centreX, centreY + 90), "LOAD GAME");
	elements["settings"] = new Button(Vector2D(centreX, centreY + 150), "SETTINGS");
	elements["quit"] = new Button(Vector2D(centreX, centreY + 210), "QUIT GAME");

	Init();
}

void MainMenu::Init()
{
	//graphics->WindowTitle("Main menu");
	graphics->SetBackgroundColour(0x00, 0x00, 0x00, 0xff);
}

MainMenu::~MainMenu()
{
	Exit();

	states = nullptr;
	graphics = nullptr;
	timer = nullptr;
	input = nullptr;
	audio = nullptr;

	for (auto& e : elements)
	{
		delete e.second;
	}
	elements.clear();
}

void MainMenu::Exit()
{
	exit = true;
}

void MainMenu::EarlyUpdate()
{
	if (input->KeyPressed(SDL_SCANCODE_ESCAPE))
	{
		endGame = true;
	}

	if (input->KeyPressed(SDL_SCANCODE_BACKSPACE))
	{
		Exit();
	}
}

void MainMenu::Update()
{
	for (auto& e : elements)
	{
		if (e.second->IsVisible()) e.second->Update();
	}

	if (((Button*)elements["start"])->IsActivated())
	{
		states->StartState<TestLevel>();
	}

	if (((Button*)elements["load"])->IsActivated())
	{
		graphics->ToggleFullscreen();
	}

	if (((Button*)elements["settings"])->IsActivated())
	{
		states->StartState<SettingsMenu>();
	}

	if (((Button*)elements["quit"])->IsActivated())
	{
		endGame = true;
	}
}

void MainMenu::LateUpdate()
{

}

void MainMenu::Render()
{
	for (auto& e : elements)
	{
		if (e.second->IsVisible()) e.second->Draw();
	}
}

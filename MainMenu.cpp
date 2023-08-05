#include "MainMenu.hpp"

MainMenu::MainMenu()
	: State()
{
	states = States::GetInstance();
	
	elements["title"] = new UIText(Vector2D(400, 200), "GAME LOGO HERE", 36);

	elements["start"] = new Button(Vector2D(400, 350), "NEW GAME");
	elements["load"] = new Button(Vector2D(400, 400), "LOAD GAME");
	elements["settings"] = new Button(Vector2D(400, 450), "SETTINGS");
	elements["quit"] = new Button(Vector2D(400.0f, 500.0f), "QUIT GAME");

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

	if (input->KeyPressed(SDL_SCANCODE_SPACE))
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

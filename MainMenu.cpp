#include "MainMenu.hpp"

MainMenu::MainMenu()
	: State()
{
	states = States::GetInstance();
	
	elements["test button"] = new Button(Vector2D(400.0f, 300.0f), "Test button");

	Init();
}

void MainMenu::Init()
{
	graphics->WindowTitle("Main menu");
	graphics->SetBackgroundColour(0x00, 0x00, 0x00, 0xff);
}

MainMenu::~MainMenu()
{
	Exit();
}

void MainMenu::Exit()
{
	exit = true;

	// Clean things up!!
	states = nullptr;
	graphics = nullptr;
	timer = nullptr;
	input = nullptr;
	audio = nullptr;
}

void MainMenu::EarlyUpdate()
{
	if (input->KeyPressed(SDL_SCANCODE_ESCAPE))
	{
		Exit();
	}
}

void MainMenu::Update()
{
	for (auto& e : elements)
	{
		e.second->Update();
	}

	if (((Button*)elements["test button"])->TestActivated())
	{
		states->StartState<TestLevel>();
	}
}

void MainMenu::LateUpdate()
{

}

void MainMenu::Render()
{
	for (auto& e : elements)
	{
		e.second->Draw();
	}
}

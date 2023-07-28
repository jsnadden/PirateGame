#include "MainMenu.hpp"

MainMenu::MainMenu()
	: State()
{
	states = States::GetInstance();

	graphics->SetBackgroundColour(0x00, 0x00, 0x00, 0xff);
	
	elements["test button"] = new Button(Vector2D(400.0f, 300.0f), "Test button");
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

#include "MainMenu.hpp"

MainMenu::MainMenu()
	: State()
{
	states = States::GetInstance();

	graphics->SetBackgroundColour(0x00, 0x00, 0x00, 0xff);

	counter = 2.0f;
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
}

void MainMenu::EarlyUpdate()
{

}

void MainMenu::Update()
{
	counter -= timer->DeltaTime();

	if (counter <= 0)
	{
		states->StartState<TestLevel>();
	}

}

void MainMenu::LateUpdate()
{

}

void MainMenu::Render()
{

}

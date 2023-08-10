#include "src/Managers/States.hpp"

States* States::instance = nullptr;

States::States()
{}

States::~States()
{
	while (!stateStack.empty())
	{
		EndCurrentState();
	}
}

States* States::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new States();
	}

	return instance;
}

void States::Release()
{
	delete instance;
	instance = nullptr;
}

void States::EndCurrentState()
{
	delete stateStack.top();
	stateStack.pop();
}

void States::ExitPoll()
{
	if (stateStack.top()->HasExited())
	{
		EndCurrentState();
		if (!stateStack.empty()) { stateStack.top()->Init(); }
	}
}

bool States::KillSwitch()
{
	if (stateStack.empty()) return true;
	else return stateStack.top()->HasEndedGame();
}



void States::EarlyUpdate()
{
	if (!stateStack.empty())
	{
		stateStack.top()->EarlyUpdate();

		ExitPoll();
	}
}

void States::Update()
{
	if (!stateStack.empty())
	{
		stateStack.top()->Update();

		ExitPoll();
	}
}

void States::LateUpdate()
{
	if (!stateStack.empty())
	{
		stateStack.top()->LateUpdate();

		ExitPoll();
	}
}

void States::Render()
{
	if (!stateStack.empty())
	{
		stateStack.top()->Render();

		ExitPoll();
	}
}

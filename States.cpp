#include "States.hpp"

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

void States::CheckEndState()
{
	if (stateStack.top()->HasExited())
	{
		EndCurrentState();
		if (!stateStack.empty()) { stateStack.top()->Init(); }
	}
}

bool States::EarlyUpdate()
{
	if (!stateStack.empty())
	{
		stateStack.top()->EarlyUpdate();

		CheckEndState();

		return false;
	}
	else
	{
		return true;
	}
}

bool States::Update()
{
	if (!stateStack.empty())
	{
		stateStack.top()->Update();

		CheckEndState();

		return false;
	}
	else
	{
		return true;
	}
}

bool States::LateUpdate()
{
	if (!stateStack.empty())
	{
		stateStack.top()->LateUpdate();

		CheckEndState();

		return false;
	}
	else
	{
		return true;
	}
}

bool States::Render()
{
	if (!stateStack.empty())
	{
		stateStack.top()->Render();

		CheckEndState();

		return false;
	}
	else
	{
		return true;
	}
}

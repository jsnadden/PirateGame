#include "States.hpp"

States* States::instance = nullptr;

States::States()
{}

States::~States()
{
	while (!stateStack.empty())
	{
		stateStack.top()->Exit();
		delete stateStack.top();
		stateStack.pop();
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

bool States::EarlyUpdate()
{
	if (!stateStack.empty())
	{
		stateStack.top()->EarlyUpdate();

		// Check if current state needs to be ended
		if (stateStack.top()->HasExited())
		{
			delete stateStack.top();
			stateStack.pop();
		}

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

		// Check if current state needs to be ended
		if (stateStack.top()->HasExited())
		{
			delete stateStack.top();
			stateStack.pop();
		}

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

		// Check if current state needs to be ended
		if (stateStack.top()->HasExited())
		{
			delete stateStack.top();
			stateStack.pop();
		}

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

		// Check if current state needs to be ended
		if (stateStack.top()->HasExited())
		{
			delete stateStack.top();
			stateStack.pop();
		}

		return false;
	}
	else
	{
		return true;
	}
}

#pragma once
#include <stack>
#include "State.hpp"

class States
{
private:
	std::stack<State*> stateStack;

	States();
	~States();

	static States* instance;

public:

	static States* GetInstance();
	static void Release();

	template <typename T>
	void StartState()
	{
		stateStack.push(new T());
	}

	bool EarlyUpdate();
	bool Update();
	bool LateUpdate();
	bool Render();

};


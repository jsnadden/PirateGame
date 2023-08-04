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
	void EndCurrentState();
	void ExitPoll();
	bool KillSwitch();

	void EarlyUpdate();
	void Update();
	void LateUpdate();
	void Render();

};


#pragma once
#include <vector>
#include <stack>
#include <map>
#include "src/Managers/Managers.hpp"
#include "src/GUI/Button.hpp"
#include "src/GUI/UIText.hpp"

class State
{
protected:

	bool exit;
	bool endGame;
	bool paused;

	Graphics* graphics;
	Timer* timer;
	Audio* audio;
	Input* input;

	std::map<std::string, UIElement*> elements;

public:

	State();
	virtual ~State();

	const bool& HasExited();
	virtual void Exit() = 0;

	const bool& HasEndedGame();

	bool IsPaused();
	void TogglePause();

	virtual void Init() = 0;
	virtual void EarlyUpdate() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render() = 0;

};


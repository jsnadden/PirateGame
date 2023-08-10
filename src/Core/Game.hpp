#pragma once
#include "src/States/ListOfStates.hpp"

class Game
{
public:

	static Game* GetInstance();
	static void Release();

	void Run();

private:

	const int FRAME_RATE = 60;
	const float FRAME_SECS = 1.0f / FRAME_RATE;

	static Game* instance;

	bool quit;

	Graphics* graphics;
	Assets* assets;
	Input* input;
	Timer* timer;
	Audio* audio;
	Camera* camera;
	States* states;

	SDL_Event event;
	
	void InitialState();

	void LoadSettings(std::string iniPath);

	void EarlyUpdate();
	void Update();
	void LateUpdate();
	void Render();

	Game();
	~Game();

};


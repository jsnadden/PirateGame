#pragma once
#include "State.hpp"

class Game
{
public:

	static Game* GetInstance();
	static void Release();

	void Run();

	enum groupLabels : std::size_t
	{
		mapGroup,
		playerGroup,
		enemyGroup,
		colliderGroup,
		projectileGroup
	};

private:

	const int FRAME_RATE = 60;
	const float FRAME_SECS = 1.0f / FRAME_RATE;

	static Game* instance;

	bool quit;

	Graphics* graphics;
	Assets* assets;
	Input* input;
	Audio* audio;
	Camera* camera;

	SDL_Rect viewRect;

	Timer* timer;

	SDL_Event event;

	void EarlyUpdate();
	void Update();
	void LateUpdate();
	void Render();

	Game();
	~Game();

};


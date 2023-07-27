#pragma once
#include "Graphics.hpp"
#include "Assets.hpp"
#include "Input.hpp"
#include "Timer.hpp"
#include "Audio.hpp"
#include "Map.hpp"
#include "ECS.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"
#include "Camera.hpp"
#include "Collision.hpp"
#include "GameState.hpp"

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
	Timer* timer;
	Audio* audio;
	Camera* camera;

	std::stack<State*> stateStack;

	SDL_Rect viewRect;

	SDL_Event event;
	
	void InitStates();

	void EarlyUpdate();
	void Update();
	void LateUpdate();
	void Render();

	Game();
	~Game();

};


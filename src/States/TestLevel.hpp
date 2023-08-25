#pragma once
#include "src/ECS/Entity.hpp"
#include "src/ECS/Components.hpp"
#include "src/ECS/Systems/AI.hpp"
#include "src/ECS/Systems/Physics.hpp"
#include "src/ECS/Systems/Rendering.hpp"
#include "src/ECS/Systems/InputHandling.hpp"
#include "src/TileMaps/Map.hpp"
#include "src/States/State.hpp"
#include "src/Managers/States.hpp"
#include "src/Utility/Collision.hpp"
#include "src/States/ListOfStates.hpp"
#include "src/GUI/PauseMenu.hpp"
#include "src/Utility/Maths.hpp"

class TestLevel :
    public State
{
private:

    Camera* camera;
    States* states;

    entt::registry enttReg;

    const int GLOBAL_SCALE = 2;

    Entity player;

    int chunkPixelSize;
    int lastChunkX;
    int lastChunkY;

    Map* map;

    SDL_Color PAUSE_TINT = { 0x00, 0x00, 0x00, 0x80 };

    PauseMenu* pauseMenu;

public:

    TestLevel();
    ~TestLevel();

    void Exit() override;

    void Init() override;
    void EarlyUpdate() override;
    void Update() override;
    void LateUpdate() override;
    void Render() override;

    void InitMap();
    void InitPlayer();
    void InitGui();

    void ShuffleMaps();

    Entity CreateEntity(std::string tag);
};


#pragma once
#include "src/ECS/Entity.hpp"
#include "src/ECS/Components.hpp"
#include "src/ECS/Systems/Physics.hpp"
#include "src/ECS/Systems/Rendering.hpp"
#include "src/ECS/Systems/InputHandling.hpp"
#include "src/TileMaps/Map.hpp"
#include "src/States/State.hpp"
#include "src/Managers/States.hpp"
#include "src/Geometry/Collision.hpp"
#include "src/States/ListOfStates.hpp"
#include "src/GUI/PauseMenu.hpp"

class TestLevel :
    public State
{
private:

    Camera* camera;
    States* states;

    entt::registry enttReg;

    Entity player;

    Map* map0;

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

    Entity CreateEntity(std::string tag);
};


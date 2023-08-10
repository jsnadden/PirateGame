#pragma once
#include <entt.hpp>
#include "src/TileMaps/Map.hpp"
#include "src/ECS/Components.hpp"
#include "src/States/State.hpp"
#include "src/Managers/States.hpp"
#include "src/Physics/Collision.hpp"
#include "src/States/ListOfStates.hpp"
#include "src/GUI/PauseMenu.hpp"

class TestLevel :
    public State
{
private:

    Camera* camera;
    States* states;

    ECSManager manager;

    Entity* player;

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
    void HandleCollision();

    std::vector<Entity*>* tiles;
    std::vector<Entity*>* players;
    std::vector<Entity*>* enemies;
    std::vector<Entity*>* colliders;
    std::vector<Entity*>* projectiles;

    enum groupLabels : std::size_t
    {
        mapGroup,
        playerGroup,
        enemyGroup,
        colliderGroup,
        projectileGroup
    };

};


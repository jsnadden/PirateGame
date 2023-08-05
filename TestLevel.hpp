#pragma once
#include "Map.hpp"
#include "Components.hpp"
#include "State.hpp"
#include "States.hpp"
#include "Collision.hpp"
#include "ListOfStates.hpp"
#include "PauseMenu.hpp"

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


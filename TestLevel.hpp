#pragma once
#include "Map.hpp"
#include "Components.hpp"
#include "State.hpp"
#include "Collision.hpp"

class TestLevel :
    public State
{
private:

    Camera* camera;

    ECSManager manager;

    Entity* player;

    Map* map0;

    SDL_Color PAUSE_TINT = { 0x00, 0x00, 0x00, 0x80 };
    SDL_Color BLACK = { 0x00, 0x00, 0x00, 0xff };
    SDL_Color LIGHT_GREY = { 0xa0, 0xa0, 0xa0, 0xff };
    SDL_Color WHITE = { 0xff, 0xff, 0xff, 0xff };
    SDL_Color TRANSPARENT = { 0x00, 0x00, 0x00, 0x00 };

public:

    TestLevel();
    ~TestLevel();

    void Exit() override;

    void Init() override;
    void EarlyUpdate() override;
    void Update() override;
    void LateUpdate() override;
    void Render() override;

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


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

public:

    TestLevel();
    ~TestLevel();

    void Exit();

    void EarlyUpdate();
    void Update();
    void LateUpdate();
    void Render();

    enum groupLabels : std::size_t
    {
        mapGroup,
        playerGroup,
        enemyGroup,
        colliderGroup,
        projectileGroup
    };

};


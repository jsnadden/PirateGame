#pragma once
#include "State.hpp"
#include "Camera.hpp"

class GameState :
    public State
{
private:

    Camera* camera;

public:

    GameState();
    ~GameState();

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


#pragma once
#include "State.hpp"

class GameState :
    public State
{
private:

    

public:

    GameState();
    ~GameState();

    void Exit();

    void EarlyUpdate();
    void Update();
    void LateUpdate();
    void Render();

};


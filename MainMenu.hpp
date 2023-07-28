#pragma once
#include "State.hpp"
#include "States.hpp"
#include "TestLevel.hpp"

class MainMenu :
    public State
{
private:

    States* states;

    float counter = 5.0f;

public:
    MainMenu();
    ~MainMenu();

    void Exit();

    void EarlyUpdate();
    void Update();
    void LateUpdate();
    void Render();

};


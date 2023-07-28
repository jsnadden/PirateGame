#pragma once
#include <vector>
#include "State.hpp"
#include "States.hpp"
#include "TestLevel.hpp"
#include "UIElement.hpp"

class MainMenu :
    public State
{
private:

    States* states;

    std::vector<UIElement*> elements;

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


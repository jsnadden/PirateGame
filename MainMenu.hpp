#pragma once
#include <map>
#include "State.hpp"
#include "States.hpp"
#include "TestLevel.hpp"
#include "Button.hpp"
#include "Vector2D.hpp"

class MainMenu :
    public State
{
private:

    States* states;

    std::map<std::string, UIElement*> elements;

public:
    MainMenu();
    ~MainMenu();

    void Exit() override;

    void Init() override;
    void EarlyUpdate() override;
    void Update() override;
    void LateUpdate() override;
    void Render() override;

};


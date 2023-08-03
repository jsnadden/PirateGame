#pragma once
#include <map>
#include "State.hpp"
#include "States.hpp"
#include "TestLevel.hpp"
#include "SettingsMenu.hpp"
#include "Vector2D.hpp"

class MainMenu :
    public State
{
private:

    States* states;

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


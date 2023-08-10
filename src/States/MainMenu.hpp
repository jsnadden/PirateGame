#pragma once
#include <map>
#include "src/States/State.hpp"
#include "src/Managers/States.hpp"
#include "src/States/TestLevel.hpp"
#include "src/States/SettingsMenu.hpp"
#include "src/Structs/Vector2D.hpp"

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


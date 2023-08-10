#pragma once
#include <map>
#include "src/States/State.hpp"
#include "src/Managers/States.hpp"
#include "src/Structs/Vector2D.hpp"
#include "src/GUI/Picker.hpp"

class SettingsMenu :
    public State
{
private:



public:
    SettingsMenu();
    ~SettingsMenu();

    void Exit() override;

    void Init() override;
    void EarlyUpdate() override;
    void Update() override;
    void LateUpdate() override;
    void Render() override;

    void SaveSettings();

};


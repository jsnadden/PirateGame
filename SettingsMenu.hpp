#pragma once
#include <map>
#include "State.hpp"
#include "States.hpp"
#include "Vector2D.hpp"
#include "Picker.hpp"

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

};


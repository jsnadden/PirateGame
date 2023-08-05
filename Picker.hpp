#pragma once
#include "UIElement.hpp"
#include "Button.hpp"
#include "UIText.hpp"
#include "Panel.hpp"

class Picker :
    public UIElement
{
private:



    Vector2D centre;
    int scale;

    int index = 0;

    std::vector<UIText*> options;
    UIText* title;
    int fontSize = 16;

    Panel* panel = nullptr;

public:

    Button* leftButton = nullptr;
    Button* rightButton = nullptr;
    

    Picker(Vector2D position, std::string t, int s = 1);
    ~Picker();

    void Update();
    void Draw();

    int Index();
    void SetIndex(int i);

    size_t OptionCount();
    UIText* CurrentOption();
    UIText* Option(size_t i);
    void AddOption(std::string name);


};


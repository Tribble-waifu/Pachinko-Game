#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"
#include <string>

class Button {
public:
    Button(Rectangle bounds, const std::string& text);

    void Update();
    bool IsClicked() const;
    bool IsHovered() const;

    Rectangle GetBounds() const;
    std::string GetText() const;

private:
    Rectangle bounds;
    std::string text;
    bool clicked;
    bool hovered;
};

#endif

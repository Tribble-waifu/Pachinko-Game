#include "button.h"

Button::Button(Rectangle b, const std::string& t)
    : bounds(b), text(t), clicked(false), hovered(false) {}

void Button::Update() {
    hovered = CheckCollisionPointRec(GetMousePosition(), bounds);
    clicked = hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool Button::IsClicked() const {
    return clicked;
}

bool Button::IsHovered() const {
    return hovered;
}

Rectangle Button::GetBounds() const {
    return bounds;
}

std::string Button::GetText() const {
    return text;
}

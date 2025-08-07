#ifndef PIN_H
#define PIN_H

#include "raylib.h"

class Pin {
public:
    Pin(Vector2 position, float radius);

    void Draw() const;

    Vector2 GetPosition() const;
    float GetRadius() const;

private:
    Vector2 position;
    float radius;
};

#endif // PIN_H

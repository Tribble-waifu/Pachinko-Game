#ifndef SLOT_H
#define SLOT_H

#include "raylib.h"
#include "raymath.h"

class Slot {
public:
    Slot(Rectangle rect);
    
    void Draw() const;
    bool CheckHit(Vector2 ballPos, float ballRadius) const;

    Rectangle GetRect() const;

private:
    Rectangle slotRect;
};

#endif // SLOT_H

#include "slot.h"

Slot::Slot(Rectangle rect) : slotRect(rect) {}

void Slot::Draw() const {
    DrawRectangleRec(slotRect, GOLD);
    DrawRectangleLinesEx(slotRect, 2, BROWN);
}

bool Slot::CheckHit(Vector2 ballPos, float ballRadius) const {
    // 用圆和矩形的碰撞检测
    float closestX = Clamp(ballPos.x, slotRect.x, slotRect.x + slotRect.width);
    float closestY = Clamp(ballPos.y, slotRect.y, slotRect.y + slotRect.height);

    float dx = ballPos.x - closestX;
    float dy = ballPos.y - closestY;

    return (dx * dx + dy * dy) <= (ballRadius * ballRadius);
}

Rectangle Slot::GetRect() const {
    return slotRect;
}

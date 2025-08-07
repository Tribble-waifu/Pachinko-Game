#include "pin.h"

Pin::Pin(Vector2 pos, float r)
    : position(pos), radius(r) {}

void Pin::Draw() const {
    // 外层轮廓
    DrawCircleV(position, radius + 1, BLACK);

    // 主体钉子
    DrawCircleV(position, radius, GRAY);

    // 中心亮点（立体感）
    DrawCircleV({ position.x - radius * 0.2f, position.y - radius * 0.2f }, radius * 0.3f, LIGHTGRAY);
}

Vector2 Pin::GetPosition() const {
    return position;
}

float Pin::GetRadius() const {
    return radius;
}

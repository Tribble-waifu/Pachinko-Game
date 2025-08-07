#include "ball.h"

Ball::Ball(Vector2 startPos, float r)
    : position(startPos), velocity({0, 0}), radius(r), gravity(600.0f), active(true) {}

void Ball::Update(float deltaTime) {
    if (!active) return;

    // 简单重力影响
    velocity.y += gravity * deltaTime;

    // 更新位置
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
}

void Ball::Draw() const {
    if (!active) return;

    DrawGlossyCircle(); // 更漂亮的小钢珠样式
}

void Ball::DrawGlossyCircle() const {
    // 主体球体
    DrawCircleV(position, radius, DARKGRAY);

    // 内部光泽渐变（白色偏移圆）
    Vector2 glossPos = {
        position.x - radius * 0.4f,
        position.y - radius * 0.4f
    };
    DrawCircleV(glossPos, radius * 0.4f, ColorAlpha(WHITE, 0.5f));
}

void Ball::Reset(Vector2 newPos) {
    position = newPos;
    velocity = {0, 0};
    active = true;
}

bool Ball::CheckCollisionWithPin(const Vector2& pinPos, float pinRadius) {
    if (!active) return false;

    float distance = Vector2Distance(position, pinPos);
    float overlap = (radius + pinRadius) - distance;

    if (overlap > 0.0f) {
        // 法线方向（从钉子指向球）
        Vector2 normal = Vector2Normalize(Vector2Subtract(position, pinPos));

        // 推出重叠
        position = Vector2Add(position, Vector2Scale(normal, overlap));

        // 反射速度
        velocity = Vector2Reflect(velocity, normal);

        // 加一点摩擦
        velocity = Vector2Scale(velocity, 0.9f);

        float randomPush = GetRandomValue(-30, 30); // 例如 -30 ~ 30 px/s
        velocity.x += randomPush;

        return true;
    }
    return false;
}


Vector2 Ball::GetPosition() const {
    return position;
}

Vector2 Ball::GetVelocity() const {
    return velocity;
}

float Ball::GetRadius() const {
    return radius;
}

void Ball::SetPosition(Vector2 newPos) {
    position = newPos;
}

void Ball::SetVelocity(Vector2 vel) {
    velocity = vel;
}

bool Ball::IsActive() const {
    return active;
}

void Ball::SetActive(bool state) {
    active = state;
}

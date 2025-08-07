#ifndef BALL_H
#define BALL_H

#include "raylib.h"
#include <cmath> // for std::fmax
#include "raymath.h" // for Vector2 operations


class Ball {
public:
    Ball(Vector2 startPos, float radius);

    void Update(float deltaTime);
    void Draw() const;
    void Reset(Vector2 newPos);

    bool CheckCollisionWithPin(const Vector2& pinPos, float pinRadius);
    void SetPosition(Vector2 newPos);

    Vector2 GetPosition() const;
    Vector2 GetVelocity() const;
    float GetRadius() const;

    void SetVelocity(Vector2 vel);

    bool IsActive() const;
    void SetActive(bool active);

private:
    Vector2 position;
    Vector2 velocity;
    float radius;
    float gravity;

    bool active;

    // 私有辅助方法
    void DrawGlossyCircle() const;
};

#endif // BALL_H

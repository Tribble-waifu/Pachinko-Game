#include "reward_effect.h"
#include <cmath>
#include <vector>

namespace RewardEffect {

    static float timer = 0.0f;
    static float duration = 2.0f;
    static bool active = false;

    static Sound winSound;

    void Init() {
        winSound = LoadSound("assets/sounds/win.wav");
    }

    void Unload() {
        UnloadSound(winSound);
    }

    void Trigger() {
        timer = duration;
        active = true;
        PlaySound(winSound);
    }

    void Update(float deltaTime) {
        if (active) {
            timer -= deltaTime;
            if (timer <= 0.0f) {
                active = false;
            }
        }
    }

    void Draw() {
        if (!active) return;

        int numLights = 12;
        float radius = 100.0f;
        Vector2 center = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

        for (int i = 0; i < numLights; ++i) {
            float angle = (2 * PI / numLights) * i;
            float x = center.x + radius * std::cos(angle);
            float y = center.y + radius * std::sin(angle);

            int colorPhase = (i + static_cast<int>(timer * 10)) % 6;
            Color lightColor;

            switch (colorPhase) {
                case 0: lightColor = RED; break;
                case 1: lightColor = ORANGE; break;
                case 2: lightColor = YELLOW; break;
                case 3: lightColor = GREEN; break;
                case 4: lightColor = BLUE; break;
                case 5: lightColor = PURPLE; break;
                default: lightColor = WHITE; break;
            }

            DrawCircle(static_cast<int>(x), static_cast<int>(y), 12, lightColor);
        }
    }

    bool IsActive() {
        return active;
    }

}

#include "ui.h"
#include "raylib.h"

namespace UI {

    // ✅ 按钮绘制函数
    void DrawButton(Rectangle bounds, const std::string& text, bool hovered) {
        Color fillColor = hovered ? GRAY : LIGHTGRAY;
        DrawRectangleRec(bounds, fillColor);
        DrawRectangleLinesEx(bounds, 2, DARKGRAY);

        int fontSize = 20;
        int textWidth = MeasureText(text.c_str(), fontSize);
        float textX = bounds.x + (bounds.width - textWidth) / 2;
        float textY = bounds.y + (bounds.height - fontSize) / 2;

        DrawText(text.c_str(), static_cast<int>(textX), static_cast<int>(textY), fontSize, BLACK);
    }

    // ✅ 居中绘制文字（基于中心点）
    void DrawTextCentered(const std::string& text, Vector2 center, int fontSize, Color color) {
        int textWidth = MeasureText(text.c_str(), fontSize);
        int textX = static_cast<int>(center.x - textWidth / 2);
        int textY = static_cast<int>(center.y - fontSize / 2);
        DrawText(text.c_str(), textX, textY, fontSize, color);
    }

    // ✅ 常规标签文字（用于左上角说明等）
    void DrawLabel(const std::string& text, Vector2 position, int fontSize, Color color) {
        DrawText(text.c_str(), static_cast<int>(position.x), static_cast<int>(position.y), fontSize, color);
    }

    // ✅ HUD 绘制函数
    void DrawHUD(int ballsRemaining, int score, bool showCongrats, int screenWidth) {
        // 左上角提示文字
        DrawLabel("Press R to Reset | F11 to Toggle Fullscreen", { 10, 10 }, 20, BLACK);

        // 显示剩余球数
        std::string ballText = "Balls Remaining: " + std::to_string(ballsRemaining);
        DrawLabel(ballText, { 10, 40 }, 20, BLACK);

        // 显示分数（右上角）
        std::string scoreText = "Score: " + std::to_string(score);
        int scoreTextWidth = MeasureText(scoreText.c_str(), 20);
        DrawLabel(scoreText, { static_cast<float>(screenWidth - scoreTextWidth - 10), 10 }, 20, DARKBLUE);

        // 中间 congratulation 提示
        if (showCongrats) {
            Vector2 center = { static_cast<float>(screenWidth) / 2.0f, 100.0f };
            DrawTextCentered("CONGRATULATIONS!", center, 30, MAROON);
        }
    }


    bool IsButtonClicked(Rectangle bounds, const std::string& text) {
        Vector2 mouse = GetMousePosition();
        bool hovered = CheckCollisionPointRec(mouse, bounds);

        DrawRectangleRec(bounds, hovered ? LIGHTGRAY : GRAY);
        DrawRectangleLinesEx(bounds, 2, BLACK);

        int textWidth = MeasureText(text.c_str(), 20);
        int textX = bounds.x + (bounds.width - textWidth) / 2;
        int textY = bounds.y + (bounds.height - 20) / 2;
        DrawText(text.c_str(), textX, textY, 20, BLACK);

        return hovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
    }

    bool IsMouseOver(Rectangle bounds) {
        Vector2 mouse = GetMousePosition();
        return CheckCollisionPointRec(mouse, bounds);
    }


}

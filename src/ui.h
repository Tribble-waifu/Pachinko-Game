#ifndef UI_H
#define UI_H

#include "raylib.h"
#include <string>

namespace UI {
    
    // 画带边框的按钮（hovered 控制颜色）
    void DrawButton(Rectangle bounds, const std::string& text, bool hovered = false);


    // 居中绘制文字（文字以 center 为中心点）
    void DrawTextCentered(const std::string& text, Vector2 center, int fontSize, Color color);

    // 左上角对齐绘制文字（快速显示状态用）
    void DrawLabel(const std::string& text, Vector2 position, int fontSize = 20, Color color = DARKGRAY);

    // 绘制 HUD（例如状态、剩余球数、提示等）
    void DrawHUD(int ballsRemaining, int score, bool showCongrats, int screenWidth);

    // 可以根据需要添加更多 HUD 参数，比如分数、游戏模式等
    bool IsButtonClicked(Rectangle bounds, const std::string& text);
    bool IsMouseOver(Rectangle bounds);

}

#endif // UI_H

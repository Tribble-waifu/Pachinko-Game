#include "menu.h"

Menu::Menu(int screenWidth, int screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight),
      startGame(false), exitGame(false), goToAdmin(false)
{
    float btnWidth = 200;
    float btnHeight = 50;
    float spacing = 20;
    float centerX = screenWidth / 2.0f - btnWidth / 2.0f;
    float startY = screenHeight / 2.0f - (btnHeight + spacing) * 1.5f;

    startBtn = { centerX, startY, btnWidth, btnHeight };
    settingsBtn = { centerX, startY + btnHeight + spacing, btnWidth, btnHeight };
    exitBtn = { centerX, startY + 2 * (btnHeight + spacing), btnWidth, btnHeight };

    float adminWidth = 120;
    float adminHeight = 30;
    adminBtn = { screenWidth - adminWidth - 20, screenHeight - adminHeight - 20, adminWidth, adminHeight };
}

void Menu::Update() {
    startGame = exitGame = goToAdmin = false;

    UpdateButtons();
}

void Menu::UpdateButtons() {
    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, startBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        startGame = true;
    } else if (CheckCollisionPointRec(mouse, exitBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        exitGame = true;
    } else if (CheckCollisionPointRec(mouse, adminBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        goToAdmin = true;
    }
}

void Menu::Draw() const {
    ClearBackground(RAYWHITE);

    DrawText("Pachinko Game", screenWidth/2 - MeasureText("Pachinko Game", 40)/2, 100, 40, DARKBLUE);

    Vector2 mouse = GetMousePosition();

    DrawButton(startBtn, "Start", CheckCollisionPointRec(mouse, startBtn));
    DrawButton(settingsBtn, "Settings", CheckCollisionPointRec(mouse, settingsBtn));
    DrawButton(exitBtn, "Exit", CheckCollisionPointRec(mouse, exitBtn));

    DrawRectangleRec(adminBtn, CheckCollisionPointRec(mouse, adminBtn) ? Fade(DARKGRAY, 0.4f) : Fade(GRAY, 0.3f));
    DrawText("Admin", adminBtn.x + 10, adminBtn.y + 5, 20, BLACK);
}


bool Menu::IsMouseOver(Rectangle rect) const {
    return CheckCollisionPointRec(GetMousePosition(), rect);
}

void Menu::DrawButton(const Rectangle& rect, const char* text, bool hovered) const {
    Color bgColor = hovered ? DARKGRAY : GRAY;
    DrawRectangleRec(rect, bgColor);
    DrawText(text, rect.x + rect.width / 2 - MeasureText(text, 20)/2, rect.y + 12, 20, WHITE);
}

bool Menu::ShouldStartGame() const {
    return startGame;
}

bool Menu::ShouldExit() const {
    return exitGame;
}

bool Menu::EnteringAdminLogin() const {
    return goToAdmin;
}

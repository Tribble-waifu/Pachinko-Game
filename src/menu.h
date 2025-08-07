#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include <string>

class Menu {
public:
    Menu(int screenWidth, int screenHeight);
    
    void Update();
    void Draw() const;

    bool ShouldStartGame() const;
    bool ShouldExit() const;
    bool EnteringAdminLogin() const;

private:
    int screenWidth;
    int screenHeight;

    Rectangle startBtn;
    Rectangle settingsBtn;
    Rectangle exitBtn;
    Rectangle adminBtn;

    bool startGame;
    bool exitGame;
    bool goToAdmin;

    void UpdateButtons();
    bool IsMouseOver(Rectangle rect) const;
    void DrawButton(const Rectangle& rect, const char* text, bool hovered) const;
};

#endif // MENU_H

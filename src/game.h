#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "ui.h"
#include "ball.h"
#include "pin.h"
#include "slot.h"
#include "button.h"
#include "menu.h"
#include "reward_effect.h"
#include "reward_system.h"
#include "state.h"
#include <vector>
#include <string>
#include <queue>

class Game {
public:
    Game(int screenWidth, int screenHeight);
    ~Game();

    void Update(float deltaTime);
    void Draw() const;

    // ✅ 摆动幅度/速度控制（Admin用）
    void SetSwingAmplitude(float amplitude);
    void SetSwingSpeed(float speed);
    float GetSwingAmplitude() const;
    float GetSwingSpeed() const;

private:
    int screenWidth;
    int screenHeight;

    GameState currentState;


    // ========== 游戏内容 ==========
    Ball ball;
    std::vector<Pin> pins;
    bool ballReleased;

    float swingAmplitude;  // 摆动幅度（Admin 可调）
    float swingSpeed;      // 摆动速度（Admin 可调）


    // ========== 发射模式 ==========
    // 支持多球发射模式
    std::vector<Ball> activeBalls;
    std::vector<Button> buttons;

    bool multiShotMode = false;
    int ballsToFire = 0;
    float timeSinceLastShot = 0.0f;
    float shotInterval = 0.3f; // 每 0.3 秒发一颗
    Vector2 multiShotStartPos;  // 多发射模式的起始位置

    // ========== 中奖槽位 ==========
    Slot* centralSlot;  // 指针或对象均可
    bool showCongrats = false;
    float congratsTimer = 0.0f;

    // ========== 中奖逻辑 ==========
    LotteryState lotteryState = LotteryState::IDLE;
    std::string currentResult = "";
    // 抽奖系统相关变量
    std::queue<std::string> pendingSpins;
    bool waitingForSpinInput = false;
    float resultDisplayTimer = 0.0f;

    float spinTimer = 0.0f;
    int rewardPoints = 0;

    int playerScore = 0;  // 当前累计得分
    std::string lastResult;  // 上一次抽奖结果（用于视觉显示）

    // ========== 菜单 ==========
    Menu menu;

    // ========== 音乐 ==========
    Music backgroundMusic;
    Sound ballHitSound;
    Sound buttonClickSound;

    float musicVolume = 0.15f;  // 初始音量（0.0 ~ 1.0）

    // ========== Admin 登录 ==========
    std::string adminUsername;
    std::string adminPassword;
    char usernameInput[32];
    char passwordInput[32];
    bool loginFailed;

    // ========== 内部逻辑 ==========
    void InitPins();
    void ResetGame();

    void UpdateMenu();
    void DrawMenu() const;

    void UpdateGame(float deltaTime);
    void DrawGame() const;

    void UpdateAdminLogin();
    void DrawAdminLogin() const;

    void ToggleFullscreenIfRequested();
};

#endif // GAME_H

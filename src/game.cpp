#include "game.h"
#include <cmath>
#include <cstring>

Game::Game(int w, int h)
    : screenWidth(w), screenHeight(h),
      currentState(GameState::MENU),
      ball(Vector2{w / 2.0f, 50}, 6.0f),
      pins(),
      ballReleased(false),
      menu(w, h),
      swingAmplitude(250.0f),     // 默认摆动幅度
      swingSpeed(1.0f),           // 默认摆动速度
      adminUsername("admin"),
      adminPassword("1234"),
      loginFailed(false)
{
    usernameInput[0] = '\0';
    passwordInput[0] = '\0';

    InitPins();
    InitAudioDevice();
    RewardEffect::Init();
    backgroundMusic = LoadMusicStream("assets/music/background.mp3");
    ballHitSound = LoadSound("assets/sounds/ball_hit.wav");
    PlayMusicStream(backgroundMusic);
    SetMusicVolume(backgroundMusic, musicVolume);

    buttons.emplace_back(Rectangle{ 20.0f, static_cast<float>(screenHeight - 60), 100.0f, 40.0f }, "Single");
    buttons.emplace_back(Rectangle{ 140.0f, static_cast<float>(screenHeight - 60), 100.0f, 40.0f }, "10 Shot");
    
    centralSlot = new Slot(Rectangle{
        screenWidth / 2.0f - 40.0f,  // X: 居中
        screenHeight - 120.0f,        // Y: 底部偏上
        40.0f,                       // 宽
        40.0f                        // 高
    });
}

Game::~Game() {
    if (centralSlot) {
        delete centralSlot;
        centralSlot = nullptr;
    }
    RewardEffect::Unload();
    UnloadMusicStream(backgroundMusic);
    CloseAudioDevice();  // 可选，仅当程序完全退出时调用
}

void Game::Update(float deltaTime) {
    ToggleFullscreenIfRequested();

    switch (currentState) {
        case GameState::MENU:
            UpdateMenu();
            break;
        case GameState::PLAYING:
            UpdateGame(deltaTime);
            UpdateMusicStream(backgroundMusic);
            break;
        case GameState::ADMIN_LOGIN:
            UpdateAdminLogin();
            break;
        case GameState::EXIT:
            CloseWindow();
            break;
        default:
            break;
    }
}

void Game::Draw() const {
    switch (currentState) {
        case GameState::MENU:
            DrawMenu();
            break;
        case GameState::PLAYING:
            DrawGame();
            break;
        case GameState::ADMIN_LOGIN:
            DrawAdminLogin();
            break;
        default:
            break;
    }
}

void Game::ToggleFullscreenIfRequested() {
    if (IsKeyPressed(KEY_F11)) {
        ToggleFullscreen();
    }
}

void Game::UpdateMenu() {
    menu.Update();

    if (menu.ShouldStartGame()) {
        ResetGame();
        currentState = GameState::PLAYING;
    } else if (menu.ShouldExit()) {
        currentState = GameState::EXIT;
    } else if (menu.EnteringAdminLogin()) {
        currentState = GameState::ADMIN_LOGIN;
    }
}

void Game::DrawMenu() const {
    menu.Draw();
}

void Game::UpdateGame(float deltaTime) {
    if (IsKeyPressed(KEY_R)) {
        ResetGame();
    }

    // === 初始球摆动（视觉）===
    if (!ballReleased && ballsToFire == 0 && lotteryState == LotteryState::IDLE) {
        static float time = 0.0f;
        time += deltaTime;

        float centerX = screenWidth / 2.0f;
        float x = centerX + swingAmplitude * std::sin(time * swingSpeed);
        Vector2 pos = ball.GetPosition();
        pos.x = x;
        ball.SetPosition(pos);
    }

    // === 按钮（发射按钮）===
    for (Button& btn : buttons) {
        btn.Update();
    }

    if (buttons[0].IsClicked() && lotteryState == LotteryState::IDLE) {  // 单发
        if (ballsToFire == 0) {
            Ball newBall(ball.GetPosition(), 6.0f);
            newBall.SetVelocity({ 0, 0 });
            activeBalls.push_back(newBall);

            ballsToFire = 0;
            multiShotMode = false;
            timeSinceLastShot = 0.0f;
        }
    }

    if (buttons[1].IsClicked() && lotteryState == LotteryState::IDLE) {  // 10 连发
        if (ballsToFire == 0) {
            ballsToFire = 10;
            multiShotMode = true;
            timeSinceLastShot = 0.0f;
        }
    }

    // === 连发发射逻辑 ===
    timeSinceLastShot += deltaTime;
    if (ballsToFire > 0 && timeSinceLastShot >= shotInterval) {
        Ball newBall(ball.GetPosition(), 6.0f);
        newBall.SetVelocity({ 0, 0 });
        activeBalls.push_back(newBall);
        ballsToFire--;
        timeSinceLastShot = 0.0f;
    }

    // === 更新球 ===
    for (int i = 0; i < activeBalls.size();) {
        Ball& b = activeBalls[i];
        b.Update(deltaTime);

        for (const Pin& pin : pins) {
            if (b.CheckCollisionWithPin(pin.GetPosition(), pin.GetRadius())) {
                PlaySound(ballHitSound);
            }
        }

        // 墙壁反弹
        const float wallMargin = 100.0f;
        const float wallWidth = 50.0f;
        float leftWall = wallMargin + wallWidth;
        float rightWall = screenWidth - wallMargin - wallWidth;

        Vector2 pos = b.GetPosition();
        Vector2 vel = b.GetVelocity();
        float radius = b.GetRadius();

        if (pos.x - radius < leftWall) {
            PlaySound(ballHitSound);
            pos.x = leftWall + radius;
            vel.x *= -0.5f;
        } else if (pos.x + radius > rightWall) {
            PlaySound(ballHitSound);
            pos.x = rightWall - radius;
            vel.x *= -0.5f;
        }

        b.SetPosition(pos);
        b.SetVelocity(vel);

        // 删除出界球
        if (pos.y - radius > screenHeight + 50) {
            activeBalls.erase(activeBalls.begin() + i);
        } else {
            i++;
        }
    }

    // === 命中 Slot ===
    if (lotteryState == LotteryState::IDLE && centralSlot) {
        for (int i = 0; i < activeBalls.size(); ++i) {
            Ball& b = activeBalls[i];
            if (centralSlot->CheckHit(b.GetPosition(), b.GetRadius())) {
                std::string result = RewardSystem::GenerateResult();
                pendingSpins.push(result);

                showCongrats = true;
                RewardEffect::Trigger();
                congratsTimer = 2.0f;

                activeBalls.erase(activeBalls.begin() + i);
                break;
            }
        }
    }

    // === 等待玩家点击 Start Spin ===
    if (!pendingSpins.empty() && lotteryState == LotteryState::IDLE && !waitingForSpinInput) {
        waitingForSpinInput = true;
        lotteryState = LotteryState::WAITING_INPUT;
    }

    if (lotteryState == LotteryState::WAITING_INPUT) {
        Rectangle spinRect = { screenWidth / 2.0f - 80, screenHeight / 2.0f - 40, 160, 60 };
        if (UI::IsButtonClicked(spinRect, "Start Spin")) {
            lastResult = pendingSpins.front();
            pendingSpins.pop();

            int reward = RewardSystem::GetRewardValue(lastResult);
            playerScore += reward;

            lotteryState = LotteryState::RESULT_SHOWN;
            resultDisplayTimer = 2.0f;
            waitingForSpinInput = false;
        }
    }

    // === 显示结果计时器 ===
    if (lotteryState == LotteryState::RESULT_SHOWN) {
        resultDisplayTimer -= deltaTime;
        if (resultDisplayTimer <= 0.0f) {
            if (!pendingSpins.empty()) {
                lotteryState = LotteryState::WAITING_INPUT;
                waitingForSpinInput = true;
            } else {
                lotteryState = LotteryState::IDLE;
            }
        }
    }

    // === congratulation 显示控制 ===
    if (showCongrats) {
        congratsTimer -= deltaTime;
        if (congratsTimer <= 0.0f) {
            showCongrats = false;
        }
    }

    // === reward 特效 ===
    RewardEffect::Update(deltaTime);
}

void Game::DrawGame() const {
    ClearBackground(RAYWHITE);

    const int wallWidth = 50;
    const int wallMargin = 100;

    // ✅ 绘制背景墙
    DrawRectangle(wallMargin, 0, wallWidth, screenHeight, DARKGRAY);
    DrawRectangle(screenWidth - wallMargin - wallWidth, 0, wallWidth, screenHeight, DARKGRAY);

    // ✅ HUD（始终绘制，包括 reset 提示 和 congratulation）
    UI::DrawHUD(ballsToFire, playerScore, showCongrats, screenWidth);

    // ✅ 中奖槽位
    if (centralSlot) {
        centralSlot->Draw();
        RewardEffect::Draw();
    }

    // ✅ 多球绘制
    for (const Ball& b : activeBalls) {
        b.Draw();
    }

    // ✅ 初始摆动球（只显示在未发射状态）
    if (!ballReleased && ballsToFire == 0) {
        ball.Draw();
    }

    // ✅ 钉子
    for (const Pin& pin : pins) {
        pin.Draw();
    }

    // ✅ 按钮（带 hover 效果）
    for (const Button& btn : buttons) {
        UI::DrawButton(btn.GetBounds(), btn.GetText(), btn.IsHovered());
    }

    if (lotteryState == LotteryState::WAITING_INPUT) {
        Rectangle spinRect = { screenWidth / 2.0f - 80, screenHeight / 2.0f - 40, 160, 60 };
        UI::DrawButton(spinRect, "Start Spin", UI::IsMouseOver(spinRect));
    }
    else if (lotteryState == LotteryState::RESULT_SHOWN) {
        UI::DrawTextCentered("Result: " + lastResult, { screenWidth / 2.0f, screenHeight / 2.0f - 80 }, 30, RED);
    }
}

void Game::UpdateAdminLogin() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentState = GameState::MENU;
        return;
    }

    int usernameLen = strlen(usernameInput);
    int passwordLen = strlen(passwordInput);
    if (passwordLen == 0) {
        loginFailed = true;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        if (std::string(usernameInput) == adminUsername && std::string(passwordInput) == adminPassword) {
            currentState = GameState::ADMIN_PANEL; // 可拓展为设置面板
        } else {
            loginFailed = true;
        }
    }

    SetMouseCursor(MOUSE_CURSOR_IBEAM);

    if (IsKeyPressed(KEY_BACKSPACE)) {
        if (usernameLen > 0) usernameInput[--usernameLen] = '\0';
    }

    int key = GetCharPressed();
    if ((key >= 32) && (key <= 125)) {
        if (usernameLen < 31) {
            usernameInput[usernameLen++] = (char)key;
            usernameInput[usernameLen] = '\0';
        }
    }

    // 密码未实现隐藏，仅示范结构
}

void Game::DrawAdminLogin() const {
    ClearBackground(BLANK);
    DrawText("Admin Login", screenWidth / 2 - 80, 100, 30, DARKBLUE);

    DrawText("Username:", screenWidth / 2 - 100, 160, 20, DARKGRAY);
    DrawRectangle(screenWidth / 2 - 100, 190, 200, 30, LIGHTGRAY);
    DrawText(usernameInput, screenWidth / 2 - 95, 195, 20, BLACK);

    DrawText("Password: (type 1234)", screenWidth / 2 - 100, 240, 20, DARKGRAY);
    DrawRectangle(screenWidth / 2 - 100, 270, 200, 30, LIGHTGRAY);
    DrawText(passwordInput, screenWidth / 2 - 95, 275, 20, BLACK);

    if (loginFailed) {
        DrawText("Login failed!", screenWidth / 2 - 60, 320, 20, RED);
    }

    DrawText("Press ESC to return", 10, screenHeight - 30, 20, GRAY);
}

void Game::InitPins() {
    pins.clear();

    const int rows = 6;
    const int pinsOdd = 9;
    const int pinsEven = 10;

    const float spacingX = 50;
    const float spacingY = 50;
    const float pinRadius = 6.0f;

    float startY = 150.0f;

    for (int row = 0; row < rows; ++row) {
        int pinCount = (row % 2 == 0) ? pinsOdd : pinsEven;

        float totalWidth = (pinCount - 1) * spacingX;
        float startX = screenWidth / 2.0f - totalWidth / 2.0f;

        for (int i = 0; i < pinCount; ++i) {
            float x = startX + i * spacingX;
            float y = startY + row * spacingY;

            pins.emplace_back(Vector2{ x, y }, pinRadius);
        }
    }
}

void Game::ResetGame() {
    ball.Reset({ screenWidth / 2.0f, 50 });
    ballReleased = false;

    activeBalls.clear();
    ballsToFire = 0;
    multiShotMode = false;
    timeSinceLastShot = 0.0f;
}


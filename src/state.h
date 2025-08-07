#ifndef STATE_H
#define STATE_H

enum class GameState {
    MENU,
    PLAYING,
    SETTINGS,
    ADMIN_LOGIN,
    ADMIN_PANEL,
    EXIT
};

enum class LotteryState {
    NONE,           // 没中奖，或还未命中 slot
    IDLE,          // 等待玩家点击按钮开始抽奖
    WAITING_INPUT,  // 命中后，等待玩家按按钮抽奖
    SPINNING,       // 正在抽奖
    RESULT_SHOWN    // 显示结果中
};


#endif // STATE_H

#ifndef REWARD_SYSTEM_H
#define REWARD_SYSTEM_H

#include <string>

namespace RewardSystem {

    // 初始化概率池和奖励表（只调用一次）
    void Init();

    // 生成抽奖结果，例如 "3I9"
    std::string GenerateResult();

    // 获取奖励值（如果有）
    int GetRewardValue(const std::string& result);
    int GetRewardForResult(const std::string& result);

    // 单独生成三个 slot 字符（用于视觉表现）
    char GetLeftChar();
    char GetMiddleChar();
    char GetRightChar();

}

#endif // REWARD_SYSTEM_H

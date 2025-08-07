#include "reward_system.h"
#include <unordered_map>
#include <random>

namespace RewardSystem {

    static std::unordered_map<std::string, int> rewardValues;

    static const std::string leftCharset   = "123456789W";
    static const std::string middleCharset = "123456789I";
    static const std::string rightCharset  = "123456789N";

    static std::default_random_engine rng(std::random_device{}());

    void Init() {
        // 奖励结果与其对应奖励值
        rewardValues.clear();
        rewardValues["111"] = 100;
        rewardValues["222"] = 200;
        rewardValues["333"] = 300;
        rewardValues["444"] = 400;
        rewardValues["555"] = 500;
        rewardValues["666"] = 600;
        rewardValues["777"] = 700;
        rewardValues["888"] = 800;
        rewardValues["999"] = 900;
        rewardValues["WIN"] = 9999;
    }

    char GetRandomChar(const std::string& charset) {
        std::uniform_int_distribution<int> dist(0, charset.size() - 1);
        return charset[dist(rng)];
    }

    char GetLeftChar() {
        return GetRandomChar(leftCharset);
    }

    char GetMiddleChar() {
        return GetRandomChar(middleCharset);
    }

    char GetRightChar() {
        return GetRandomChar(rightCharset);
    }

    std::string GenerateResult() {
        char left = GetLeftChar();
        char mid = GetMiddleChar();
        char right = GetRightChar();
        return std::string() + left + mid + right;
    }

    int GetRewardValue(const std::string& result) {
        if (rewardValues.find(result) != rewardValues.end()) {
            return rewardValues[result];
        }
        return 0;
    }

    int GetRewardForResult(const std::string& result) {
        static std::unordered_map<std::string, int> rewardTable = {
            {"111", 100},
            {"222", 100},
            {"333", 100},
            {"444", 100},
            {"555", 100},
            {"666", 100},
            {"777", 200},
            {"888", 200},
            {"999", 300},
            {"WIN", 1000}
        };

        if (rewardTable.count(result)) {
            return rewardTable[result];
        } else {
            return 0; // 非中奖结果没有奖励
        }
    }
}

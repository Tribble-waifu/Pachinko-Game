#ifndef REWARD_EFFECT_H
#define REWARD_EFFECT_H

#include "raylib.h"

namespace RewardEffect {

    void Init();                      // 初始化资源
    void Unload();                   // 卸载资源

    void Trigger();                  // 触发奖励动画和音效
    void Update(float deltaTime);    // 更新动画状态
    void Draw();                     // 绘制视觉效果

    bool IsActive();                 // 是否正在播放效果

}

#endif // REWARD_EFFECT_H

#ifndef REWARD_H
#define REWARD_H

#include "entity.h"

struct Reward
{
    int x;
    int y;
    int type;
    int active; // 1 = available , 0 = used
};

extern int rewardCount;
extern struct Reward rewards[20];

void initRewards(int count);
void placeRewards();

void checkReward(int playerIdx);
void applyReward(int playerIdx, struct Reward type);

void earthshake(void);
void moveSelectHunter(int hunterIdx);

#endif
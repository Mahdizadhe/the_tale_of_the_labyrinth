#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#include "reward.h"
#include "entity.h"
#include "game.h"
#include "map.h"
#include "movement.h"
#include "wall.h"
#include "utils.h"

//==================== Globals ====================//

int rewardCount;
struct Reward rewards[20];

//==================== Init Rewards ====================//

void initRewards(int count)
{
    if (count > 20)
        count = 20;

    rewardCount = count;

    for (int i = 0; i < rewardCount; i++)
    {
        rewards[i].active = 1;
        rewards[i].type = rand() % 4; // 0..3
    }

    placeRewards();
}

//==================== Place Rewards ====================//

void placeRewards()
{
    for (int i = 0; i < rewardCount; i++)
    {
        int x, y, ok;

        do
        {
            ok = 0;
            x = rand() % n;
            y = rand() % m;

            // core
            if (x == core.x && y == core.y)
                ok = 1;

            // players
            for (int j = 0; j < playerCount; j++)
                if (players[j].x == x &&
                    players[j].y == y)
                    ok = 1;

            // hunters
            for (int j = 0; j < hunterCount; j++)
                if (hunters[j].x == x &&
                    hunters[j].y == y)
                    ok = 1;

            // rewards overlap
            for (int j = 0; j < i; j++)
                if (rewards[j].x == x &&
                    rewards[j].y == y)
                    ok = 1;

        } while (ok);

        rewards[i].x = x;
        rewards[i].y = y;
    }
}

//==================== Check Reward ====================//

void checkReward(int playerIdx)
{
    for (int i = 0; i < rewardCount; i++)
    {
        if (!rewards[i].active)
            continue;

        if (players[playerIdx].x == rewards[i].x &&
            players[playerIdx].y == rewards[i].y)
        {
            applyReward(playerIdx, rewards[i]);
            rewards[i].active = 0;
            return;
        }
    }
}

//==================== Apply Reward ====================//

void showRewardMessage(const char *msg)
{
    initMap();
    clearScreen();
    printMap();

    printf("\n=================================\n");
    printf("%s\n", msg);
    printf("=================================\n");

    printf("Press any key to continue...");
    getch();
}

void applyReward(int playerIdx, struct Reward r)
{
    switch (r.type)
    {
    case 0:
    {
        showRewardMessage("Reward: EXTRA TURN!");
        movePlayer(playerIdx);
        break;
    }

    case 1:
    {
        players[playerIdx].tempWallCount += 2;
        showRewardMessage("Reward: +2 WALLS!");
        break;
    }

    case 2:
    {
        earthshake();
        showRewardMessage("Reward: EARTHSHAKE!");
        break;
    }

    case 3:
    {
        if (hunterCount != 0)
        {
            showRewardMessage("Reward: MOVE A HUNTER!");

            int h;
            printf("\nSelect hunter (1 to %d): ", hunterCount);
            scanf("%d", &h);
            h--;

            moveSelectHunter(h);
            break;
        }
        printf("\n No hunters found");
    }
    }

    gameState();
}

//==================== Earthshake ====================//

void earthshake(void)
{
    printf("\nEARTHSHAKE!!!\n");

    // ===== players =====
    for (int i = 0; i < playerCount; i++)
    {
        if (!players[i].alive)
            continue;

        int dirs[4], cnt = 0;

        for (int d = 0; d < 4; d++)
        {
            int nx = players[i].x + dx[d];
            int ny = players[i].y + dy[d];

            if (inRange(nx, ny) &&
                !isBlocked(players[i].x, players[i].y, nx, ny))
            {
                dirs[cnt++] = d;
            }
        }

        if (cnt > 0)
        {
            int d = dirs[rand() % cnt];

            players[i].x += dx[d];
            players[i].y += dy[d];

            clearScreen();
            initMap();
            printMap();
        }
    }

    // ===== hunters =====
    for (int i = 0; i < hunterCount; i++)
    {
        int dirs[4], cnt = 0;

        for (int d = 0; d < 4; d++)
        {
            int nx = hunters[i].x + dx[d];
            int ny = hunters[i].y + dy[d];

            if (inRange(nx, ny) &&
                !isBlocked(hunters[i].x, hunters[i].y, nx, ny))
            {
                dirs[cnt++] = d;
            }
        }

        if (cnt > 0)
        {
            int d = dirs[rand() % cnt];

            hunters[i].x += dx[d];
            hunters[i].y += dy[d];

            clearScreen();
            initMap();
            printMap();
        }
    }
}

//==================== Move Selected Hunter ====================//

void moveSelectHunter(int hunterIdx)
{
    if (hunterIdx < 0 || hunterIdx >= hunterCount)
        return;

    int hx = hunters[hunterIdx].x;
    int hy = hunters[hunterIdx].y;

    printf("\nUse arrow keys to move selected hunter...\n");

    while (1)
    {
        int ch = getch(); // Read ch press
        if (ch == 224)
            ch = getch();

        int nx = hx;
        int ny = hy;

        if (ch == 72) // UP
            nx--;
        else if (ch == 80) // DOWN
            nx++;
        else if (ch == 75) // LEFT
            ny--;
        else if (ch == 77) // RIGHT
            ny++;
        else if (ch == 'S' || ch == 's') // RIGHT
            break;

        if (inRange(nx, ny) &&
            !isBlocked(hx, hy, nx, ny))
        {
            hunters[hunterIdx].x = nx;
            hunters[hunterIdx].y = ny;
            break;
        }
    }

    clearScreen();
    initMap();
    printMap();
}

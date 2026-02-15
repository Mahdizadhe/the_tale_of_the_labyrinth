#include <conio.h>
#include <stdio.h>
#include "movement.h"
#include "entity.h"
#include "wall.h"
#include "map.h"
#include "game.h"

//====================/ Player & Hunter Utilities /====================//

// Returns the Manhattan distance from (x, y) to the nearest hunter
// If there are no hunters, returns a very large number
int nearestHunterDistance(int x, int y)
{
    if (hunterCount == 0)
        return 100000;

    int minDist = distance(x, y, hunters[0].x, hunters[0].y);
    for (int i = 1; i < hunterCount; i++)
    {
        int d = distance(x, y, hunters[i].x, hunters[i].y);
        if (d < minDist)
            minDist = d;
    }
    return minDist;
}

// Suggests the best move for a player using a simple scoring system
// Returns 0=UP, 1=DOWN, 2=LEFT, 3=RIGHT, 4=STAY
int suggestBestMove(int idx)
{
    int x = players[idx].x;
    int y = players[idx].y;

    int bestMove = 4;        // Default: stay
    int bestScore = -100000; // Very low initial score

    for (int dir = 0; dir < 5; dir++) // Check all possible moves including STAY
    {
        int nx = x;
        int ny = y;

        // Determine new coordinates based on direction
        if (dir == 0)
            nx--; // UP
        else if (dir == 1)
            nx++; // DOWN
        else if (dir == 2)
            ny--; // LEFT
        else if (dir == 3)
            ny++; // RIGHT
        // dir == 4 => STAY
        else if (dir == 4)
            continue;

        // Skip invalid or blocked moves
        if (dir != 4)
        {
            if (!inRange(nx, ny))
                continue;
            if (isBlocked(x, y, nx, ny))
                continue;
        }

        int score = 0;

        // Incentivize moving closer to the core
        score += (distance(x, y, core.x, core.y) - distance(nx, ny, core.x, core.y)) * 10;

        // Incentivize moving away from hunters
        int oldHunterDist = nearestHunterDistance(x, y);
        int newHunterDist = nearestHunterDistance(nx, ny);
        score += (newHunterDist - oldHunterDist) * 20;

        // Penalize being too close to hunters
        if (newHunterDist <= 1)
            score -= 1000;

        // Update best move
        if (score > bestScore)
        {
            bestScore = score;
            bestMove = dir;
        }
    }

    return bestMove;
}

// Finds the index of the nearest alive player to a hunter
int findNearestPlayer(int idx)
{
    int hx = hunters[idx].x;
    int hy = hunters[idx].y;
    int bestIdx = -1;
    int bestDist = 100000;

    for (int i = 0; i < playerCount; i++)
    {
        if (!players[i].alive) // Skip dead players
            continue;

        int d = distance(hx, hy, players[i].x, players[i].y);
        if (d < bestDist)
        {
            bestDist = d;
            bestIdx = i;
        }
    }
    return bestIdx;
}

// Determines the next optimal position for a hunter
struct Entity calculateHunterMove(int idx)
{
    int hx = hunters[idx].x;
    int hy = hunters[idx].y;

    struct Entity result;
    result.x = hx;
    result.y = hy;

    int p = findNearestPlayer(idx);
    if (p == -1)
        return result;

    int px = players[p].x;
    int py = players[p].y;

    int bestDist = distance(hx, hy, px, py);

    for (int dir = 0; dir < 4; dir++)
    {
        int nx = hx + dx[dir];
        int ny = hy + dy[dir];

        if (!inRange(nx, ny))
            continue;

        if (isBlocked(hx, hy, nx, ny))
            continue;

        int newDist = distance(nx, ny, px, py);

        if (newDist < bestDist)
        {
            bestDist = newDist;
            result.x = nx;
            result.y = ny;
        }
    }

    return result;
}

//====================/ Player Movement /====================//

// Handles user input for moving a player and placing temporary walls
int movePlayer(int idx)
{
    int key;
    int x = players[idx].x;
    int y = players[idx].y;

    while (1)
    {
        key = getch(); // Read key press
        if (key == 224)
            key = getch();

        int nx = x;
        int ny = y;

        // Handle movement keys
        if (key == 72)
            nx--; // UP
        else if (key == 80)
            nx++; // DOWN
        else if (key == 75)
            ny--; // LEFT
        else if (key == 77)
            ny++;                          // RIGHT
        else if (key == 'S' || key == 's') // Stay
            break;
        else if (key == 'W' || key == 'w') // Place a temporary wall
        {
            // Check if player has walls left
            if (players[idx].tempWallCount == 0)
            {
                printf("No temporary walls left!\n");
                continue;
            }

            int dir;
            printf("Wall direction: 1=UP, 2=DOWN, 3=LEFT, 4=RIGHT\n");
            scanf("%d", &dir);

            // Place the wall in the chosen direction
            if (dir == 1) // UP
            {
                if (x <= 0 || vWall[x - 1][y] || vTempWall[x - 1][y])
                {
                    printf("Invalid wall position\n");
                    continue;
                }
                vTempWall[x - 1][y] = 1;
                vTempTime[x - 1][y] = 2;
            }
            else if (dir == 2) // DOWN
            {
                if (x >= n - 1 || vWall[x][y] || vTempWall[x][y])
                {
                    printf("Invalid wall position\n");
                    continue;
                }
                vTempWall[x][y] = 1;
                vTempTime[x][y] = 2;
            }
            else if (dir == 3) // LEFT
            {
                if (y <= 0 || hWall[x][y - 1] || hTempWall[x][y - 1])
                {
                    printf("Invalid wall position\n");
                    continue;
                }
                hTempWall[x][y - 1] = 1;
                hTempTime[x][y - 1] = 2;
            }
            else if (dir == 4) // RIGHT
            {
                if (y >= m - 1 || hWall[x][y] || hTempWall[x][y])
                {
                    printf("Invalid wall position\n");
                    continue;
                }
                hTempWall[x][y] = 1;
                hTempTime[x][y] = 2;
            }
            else
            {
                printf("Invalid direction\n");
                continue;
            }

            // Reduce player's temporary wall count
            players[idx].tempWallCount--;
            break;
        }
        else if (key == 'Q' || key == 'q')
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    if (vTempWall[i][j])
                        vTempTime[i][j]++;

                    if (hTempWall[i][j])
                        hTempTime[i][j]++;
                }
            }
            saveGame();
            return -1;
        }
        else
        {
            printf("Move error: Invalid move\n");
            continue;
        }

        // Check bounds
        if (!inRange(nx, ny))
        {
            printf("Move error: Out of map\n");
            continue;
        }

        // Check for walls
        if (isBlocked(x, y, nx, ny))
        {
            printf("Move error: blocked by wall\n");
            continue;
        }

        // Apply move
        players[idx].x = nx;
        players[idx].y = ny;
        break;
    }

    return 1;
}

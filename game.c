#include "game.h"
#include "map.h"
#include "entity.h"
#include "movement.h"
#include "wall.h"
#include "utils.h"
#include "reward.h"

#include <stdio.h>
#include <conio.h>

//====================/ Game State Function /====================//

// Returns the current state of the game:
// 0 -> game continues
// 1 -> all players dead (lose)
// 2 -> any player reached the core (win)
int gameState()
{
    int aliveCount = 0;

    // Check if any hunters have captured players
    for (int i = 0; i < hunterCount; i++)
    {
        for (int j = 0; j < playerCount; j++)
        {
            if (players[j].alive &&
                hunters[i].x == players[j].x &&
                hunters[i].y == players[j].y)
            {
                // Player is caught by hunter
                players[j].alive = 0;
            }
        }
    }

    // Count the number of alive players
    for (int i = 0; i < playerCount; i++)
    {
        if (players[i].alive)
            aliveCount++;
    }

    // If no players are alive, game is lost
    if (aliveCount == 0)
        return 1;

    // Check if any alive player reached the core (win condition)
    for (int i = 0; i < playerCount; i++)
    {
        if (players[i].alive &&
            players[i].x == core.x &&
            players[i].y == core.y)
        {
            return 2; // Win
        }
    }

    // Game continues
    return 0;
}

//====================/ Save & Load Functions /====================//

// Save the current game state to a file
void saveGame(void)
{
    FILE *file = fopen("save/save.dat", "wb");

    if (file == NULL)
    {
        printf("Error saving game!\n");
        printf("Press any key to continue...");
        getch();
        return;
    }

    fwrite(&n, sizeof(int), 1, file);
    fwrite(&m, sizeof(int), 1, file);

    fwrite(&playerCount, sizeof(int), 1, file);
    fwrite(&hunterCount, sizeof(int), 1, file);
    fwrite(&wallCount, sizeof(int), 1, file);
    fwrite(&rewardCount, sizeof(int), 1, file);

    fwrite(&core, sizeof(struct Entity), 1, file);

    fwrite(&players, sizeof(struct Entity), playerCount, file);
    fwrite(&hunters, sizeof(struct Entity), hunterCount, file);

    fwrite(&hWall, sizeof(hWall), 1, file);
    fwrite(&vWall, sizeof(vWall), 1, file);

    fwrite(&hTempWall, sizeof(hTempWall), 1, file);
    fwrite(&vTempWall, sizeof(vTempWall), 1, file);

    fwrite(&hTempTime, sizeof(hTempTime), 1, file);
    fwrite(&vTempTime, sizeof(vTempTime), 1, file);

    fwrite(&rewards, sizeof(struct Reward), rewardCount, file);

    fclose(file);

    clearScreen();
    printf("=======================\n");
    printf("GAME SAVED SUCCESSFULLY\n");
    printf("=======================\n\n");
    printf("Press any key to continue...");
    getch();
}

// Load a saved game state from file
int loadGame()
{
    FILE *file = fopen("save/save.dat", "rb");

    if (file == NULL)
    {
        printf("No saved game found!\n");
        return 0;
    }

    fread(&n, sizeof(int), 1, file);
    fread(&m, sizeof(int), 1, file);

    fread(&playerCount, sizeof(int), 1, file);
    fread(&hunterCount, sizeof(int), 1, file);
    fread(&wallCount, sizeof(int), 1, file);
    fread(&rewardCount, sizeof(int), 1, file);

    fread(&core, sizeof(struct Entity), 1, file);

    fread(&players, sizeof(struct Entity), playerCount, file);
    fread(&hunters, sizeof(struct Entity), hunterCount, file);

    fread(&hWall, sizeof(hWall), 1, file);
    fread(&vWall, sizeof(vWall), 1, file);

    fread(&hTempWall, sizeof(hTempWall), 1, file);
    fread(&vTempWall, sizeof(vTempWall), 1, file);

    fread(&hTempTime, sizeof(hTempTime), 1, file);
    fread(&vTempTime, sizeof(vTempTime), 1, file);

    fread(&rewards, sizeof(struct Reward), rewardCount, file);

    fclose(file);

    printf("=========================\n");
    printf("Game Loaded Successfully!\n");
    printf("=========================\n\n");
    printf("Press any key to continue...");
    getch();
    return 1;
}

//====================/ Main Game Loop /====================//

int game()
{
    while (1)
    {
        // Initialize map and clear screen
        initMap();     // Fill map with current positions of entities and rewards
        clearScreen(); // Clear console for updated display

        //====================/ Display Game Info /====================//
        printf("CORE POSITION : [%d - %d]\n", core.x + 1, core.y + 1);
        printf("*******************************\n");

        // Print each player's position and remaining wall count
        for (int i = 0; i < playerCount; i++)
        {
            printf("PLAYER NO. %d POSITION : [%d - %d]\n",
                   i + 1,
                   players[i].x + 1,
                   players[i].y + 1);
        }

        printf("*******************************\n");

        // Print each player's temp wall count
        for (int i = 0; i < playerCount; i++)
        {
            printf("PLAYER NO. %d TEMP WALL : [%d]\n",
                   i + 1,
                   players[i].tempWallCount);
        }

        printf("*******************************\n");

        // Print hunters' positions
        for (int i = 0; i < hunterCount; i++)
        {
            printf("HUNTER NO. %d POSITION : [%d - %d]\n",
                   i + 1,
                   hunters[i].x + 1,
                   hunters[i].y + 1);
        }

        printf("*******************************\n\n");

        //====================/ Display Map /====================//
        printMap();       // Render current map
        updateTempWall(); // Update temporary walls (reduce timers)

        //====================/ Player Turns /====================//

        for (int i = 0; i < playerCount; i++)
        {
            if (!players[i].alive)
                continue; // Skip dead players

            // Suggest best move for player (AI or hint)
            int suggestion = suggestBestMove(i);
            printf("Suggested move for PLAYER %d: ", i + 1);
            if (suggestion == 0)
                printf("UP\n");
            else if (suggestion == 1)
                printf("DOWN\n");
            else if (suggestion == 2)
                printf("LEFT\n");
            else if (suggestion == 3)
                printf("RIGHT\n");
            else
                printf("STAY (S)\n");

            printf("PLAYER %d TURN\n", i + 1);

            int result = movePlayer(i); // Take player input and move

            if (result == -1)
                return -1;

            // Check game state after player moves
            int state = gameState();
            if (state != 0)
                return state;

            checkReward(i); // Check if a reward is collected
        }

        //====================/ Hunter Turns /====================//
        // Each hunter moves twice per round
        for (int step = 0; step < 2; step++)
        {
            // Calculate and update hunters's positions
            for (int i = 0; i < hunterCount; i++)
            {
                struct Entity next = calculateHunterMove(i);
                hunters[i].x = next.x;
                hunters[i].y = next.y;
            }
        }

        // Check game state after hunter moves
        int state = gameState();
        if (state != 0)
            return state;
    }
}
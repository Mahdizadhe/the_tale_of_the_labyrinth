#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#include "entity.h"
#include "map.h"
#include "wall.h"
#include "movement.h"
#include "utils.h"
#include "game.h"
#include "reward.h"

// Global loop variables
int i, j;

int main()
{
    // Seed the random number generator with current time
    srand(time(NULL));

    int code; // Stores the user's menu choice

    // Main menu loop
    while (1)
    {
        clearScreen(); // Clear console

        // Display the main menu
        printf("THE TALE OF THE LABYRINTH\nPlease enter any code:\n");
        printf("1. START GAME\n2. CONTINUE\n3. HOW TO PLAY\n4. CREDITS\n5. EXIT\n");

        scanf("%d", &code); // Get user choice

        if (code == 1) // START GAME
        {
            clearScreen();
            printf("Please enter n, m, player count, hunter count, wall count, reward count\n");

            // Read map size, entity counts, and wall/reward counts
            scanf("%d %d", &n, &m);
            scanf("%d", &playerCount);
            scanf("%d", &hunterCount);
            scanf("%d", &wallCount);
            scanf("%d", &rewardCount);

            // Place core, players, and hunters on the map
            placeCore();
            placePlayers();
            placeHunters();

            // Generate walls and ensure the map is connected
            do
            {
                // Clear previous walls
                for (i = 0; i < n; i++)
                    for (j = 0; j < m; j++)
                        vWall[i][j] = hWall[i][j] = 0;

                generateWalls(wallCount); // Generate new walls
            } while (!isConnected()); // Repeat until map is fully connected

            // Initialize rewards
            initRewards(rewardCount);

            // Start the main game loop
            int result = game();

            // After game ends, display result
            clearScreen();

            if (result == 1)
                printf("YOU LOSE\n");
            else if (result == 2)
                printf("YOU WON\n");

            printf("Press any key to back to menu\n");
            getch();
        }
        else if (code == 2) // CONTINUE (not implemented)
        {
            clearScreen();

            if (loadGame())
            {
                int result = game();

                clearScreen();

                if (result == 1)
                    printf("YOU LOSE\n");
                else if (result == 2)
                    printf("YOU WON\n");

                printf("Press any key to back to menu\n");
                getch();
            }
            else
            {
                printf("Press any key to back to menu\n");
                getch();
            }
        }
        else if (code == 3) // HOW TO PLAY
        {
            clearScreen();
            printf("Use arrow keys to move\n");
            printf("Use 'S' or 's' to skip turn (not moving)\n");
            printf("Use 'W' or 'w' and '1, 2, 3, 4' to place a wall\n");
            printf("Use 'Q' or 'q' to save the game\n");
            printf("Press any key to back to menu\n");
            getch();
        }
        else if (code == 4) // CREDITS
        {
            clearScreen();
            printf("STUDENT: MOHAMMAD MAHDIZADHE\nSTUDENT ID: 4042262277\nPress any key to back to menu\n");
            getch();
        }
        else if (code == 5) // EXIT
        {
            break; // Exit the program
        }
    }

    return 0;
}

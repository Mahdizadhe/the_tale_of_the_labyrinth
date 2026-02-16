#include "map.h"
#include "entity.h"
#include "wall.h"
#include "reward.h"
#include <stdio.h>

//====================/ Map Grid /====================//

char map[100][100]; // Stores the current visual representation of the map

int n; // Number of rows
int m; // Number of columns

//====================/ Initialize Map /====================//

void initMap()
{
    // Clear all cells
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            map[i][j] = ' ';
        }
    }

    // Place the core on the map
    map[core.x][core.y] = 'C';

    // Place alive players
    for (int i = 0; i < playerCount; i++)
    {
        if (players[i].alive)
            map[players[i].x][players[i].y] = 'P';
    }

    // Place hunters
    for (int i = 0; i < hunterCount; i++)
    {
        map[hunters[i].x][hunters[i].y] = 'H';
    }

    // Place active rewards
    for (int i = 0; i < rewardCount; i++)
    {
        if (rewards[i].active)
            map[rewards[i].x][rewards[i].y] = 'R';
    }
}

//====================/ Print Map /====================//

void printMap()
{
    for (int i = 0; i < n; i++)
    {
        // ----- Top horizontal walls for this row -----
        for (int j = 0; j < m; j++)
        {
            printf("*"); // corner / wall intersection
            if (i == 0 || hWall[i - 1][j])
                printf("---"); // permanent horizontal wall
            else if (hTempWall[i - 1][j])
                printf("~~~"); // temporary horizontal wall
            else
                printf("   "); // empty space
        }
        printf("*\n");

        // ----- Cells with vertical walls -----
        for (int j = 0; j < m; j++)
        {
            if (j == 0 || vWall[i][j - 1])
                printf("|"); // permanent horizontal wall
            else if (vTempWall[i][j - 1])
                printf("/"); // temporary vertical wall
            else
                printf(" ");           // empty space
            printf(" %c ", map[i][j]); // print cell content
        }
        printf("|\n");
    }

    // ----- Bottom horizontal walls -----
    for (int j = 0; j < m; j++)
    {
        printf("*---");
    }
    printf("*\n");
}

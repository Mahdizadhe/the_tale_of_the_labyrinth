#include "entity.h"
#include "wall.h"
#include "map.h"

#include <stdlib.h>

//====================/ Macros /====================//

// Macros to find minimum and maximum of two values
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

//====================/ Global Entities /====================//

struct Entity core;        // The "Core" target in the game
struct Entity players[10]; // Array of players (max 10)
struct Entity hunters[10]; // Array of hunters (max 10)

int playerCount; // Number of players
int hunterCount; // Number of hunters

//====================/ Utility Functions /====================//

// Calculate Manhattan distance between two points
// distance = |x1 - x2| + |y1 - y2|
int distance(int x1, int y1, int x2, int y2)
{
    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);
    return dx + dy;
}

//====================/ Entity Placement /====================//

// Randomly place the core on the map
void placeCore()
{
    core.x = rand() % n; // Random X
    core.y = rand() % m; // Random Y
}

// Randomly place players on the map
void placePlayers()
{
    for (int i = 0; i < playerCount; i++)
    {
        players[i].alive = 1;

        // Assign temporary wall count for each player (at least 1)
        players[i].tempWallCount = max(min(n, m) / 3, 1);

        int x, y, error;

        do
        {
            error = 0;

            x = rand() % n; // Random X
            y = rand() % m; // Random Y

            // Prevent spawning too close to the core
            if (distance(x, y, core.x, core.y) < 2)
            {
                error = 1;
            }

            // Prevent two players from spawning on the same cell
            for (int j = 0; j < i; j++)
            {
                if (x == players[j].x && y == players[j].y)
                {
                    error = 1;
                }
            }

        } while (error); // Repeat until a valid position is found

        // Save final position
        players[i].x = x;
        players[i].y = y;
    }
}

// Randomly place hunters on the map
void placeHunters()
{
    for (int i = 0; i < hunterCount; i++)
    {
        int x, y, error;

        do
        {
            error = 0;

            x = rand() % n; // Random X
            y = rand() % m; // Random Y

            // Prevent spawning too close to the core
            if (distance(x, y, core.x, core.y) < 2)
            {
                error = 1;
            }

            // Prevent spawning too close to any player
            for (int j = 0; j < playerCount; j++)
            {
                if (distance(x, y, players[j].x, players[j].y) < 2)
                {
                    error = 1;
                }
            }

            // Prevent two hunters from spawning in the same position
            for (int j = 0; j < i; j++)
            {
                if (x == hunters[j].x && y == hunters[j].y)
                {
                    error = 1;
                }
            }

        } while (error); // Repeat until a valid position is found

        // Save final position
        hunters[i].x = x;
        hunters[i].y = y;
    }
}
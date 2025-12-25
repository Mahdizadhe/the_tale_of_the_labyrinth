#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
// #include <ctime.h>
// #include <function.c>

void clearScreen()
{
    system("cls");
}

int main()
{
    printf("Please enter n, m, player count, player position, hunter count, hunter position, wall count, wall position\n");
    int n, m, playerCount, hunterCount, wallsCount;

    char map[100][100];

    int lightCorePosition[2];   // 1 col for light core and 2 row for X and Y
    int playersPosition[10][2]; // 10 col for playerCount and 2 row for X and Y
    int huntersPosition[10][2]; // 10 col for hunterCount and 2 row for X and Y
    int wallsPosition[100][2];  // 100 col for wallCount and 1 row X and 1 row for Y

    int vWall[100][100] = {0}; // this array is for walls between (x) and (x + 1)
    int hWall[100][100] = {0}; // this array is for walls between (y) and (y + 1)

    int i, j;
    int LCERR;
    scanf("%d %d", &n, &m);
    do
    {
        LCERR = 1;
        scanf("%d %d", &lightCorePosition[0], &lightCorePosition[1]);

        if (
            lightCorePosition[0] < 0 || lightCorePosition[0] >= n ||
            lightCorePosition[1] < 0 || lightCorePosition[1] >= m)
        {
            printf("Light core position error\nPosition must be in range 0 to %d and board 0 to %d\n", n - 1, m - 1);
            LCERR = 0;
        }
    } while (!LCERR);

    int xDistanceCore, yDistanceCore;
    int xDistancePlayer, yDistancePlayer;

    int PYERR;
    scanf("%d", &playerCount);
    for (i = 0; i < playerCount; i++)
    {
        do
        {
            PYERR = 1;
            scanf("%d %d", &playersPosition[i][0], &playersPosition[i][1]);
            if (
                playersPosition[i][0] < 0 || playersPosition[i][0] >= n ||
                playersPosition[i][1] < 0 || playersPosition[i][1] >= m)
            {
                printf("Player position error\nPosition must be in range 0 to %d and board 0 to %d\n", n - 1, m - 1);
                PYERR = 0;
            }
        } while (!PYERR);

        xDistanceCore = playersPosition[i][0] - lightCorePosition[0];
        if (xDistanceCore < 0)
        {
            xDistanceCore *= (-1);
        }
        yDistanceCore = playersPosition[i][1] - lightCorePosition[1];
        if (yDistanceCore < 0)
        {
            yDistanceCore *= (-1);
        }

        if (xDistanceCore + yDistanceCore < 2)
        {
            printf("The minimum distance to the core must be 2 units\n");
            i--;
        }
    }

    int HTERR;
    scanf("%d", &hunterCount);
    for (i = 0; i < hunterCount; i++)
    {
        do
        {
            HTERR = 1;
            scanf("%d %d", &huntersPosition[i][0], &huntersPosition[i][1]);
            if (
                huntersPosition[i][0] < 0 || huntersPosition[i][0] >= n ||
                huntersPosition[i][1] < 0 || huntersPosition[i][1] >= m)
            {
                printf("Hunter position error\nPosition must be in range 0 to %d and board 0 to %d\n", n - 1, m - 1);
                HTERR = 0;
            }
        } while (!HTERR);

        xDistanceCore = huntersPosition[i][0] - lightCorePosition[0];
        if (xDistanceCore < 0)
        {
            xDistanceCore *= (-1);
        }
        yDistanceCore = huntersPosition[i][1] - lightCorePosition[1];
        if (yDistanceCore < 0)
        {
            yDistanceCore *= (-1);
        }

        if (xDistanceCore + yDistanceCore < 2)
        {
            printf("The minimum distance to the core must be 2 units\n");
            i--;
        }

        for (j = 0; j < playerCount; j++)
        {
            xDistancePlayer = huntersPosition[i][0] - playersPosition[j][0];
            if (xDistancePlayer < 0)
            {
                xDistancePlayer *= (-1);
            }
            yDistancePlayer = huntersPosition[i][1] - playersPosition[j][1];
            if (yDistancePlayer < 0)
            {
                yDistancePlayer *= (-1);
            }

            if (xDistancePlayer + yDistancePlayer < 2)
            {
                printf("The minimum distance to the player must be 2 units\n");
                i--;
            }
        }
    }

    int WLLERR;
    scanf("%d", &wallsCount);
    for (i = 0; i < wallsCount; i++)
    {
        char wallType;

        do
        {
            WLLERR = 1;
            scanf("%d %d %c", &wallsPosition[i][0], &wallsPosition[i][1], &wallType);

            if (wallType == 'v' || wallType == 'V')
            {
                if (wallsPosition[i][0] == n - 1)
                {
                    printf("Wall error\nThe wall should not be placed of border\n");
                    WLLERR = 0;
                }
            }
            if (wallType == 'h' || wallType == 'H')
            {
                if (wallsPosition[i][1] == m - 1)
                {
                    printf("Wall error\nThe wall should not be placed of border\n");
                    WLLERR = 0;
                }
            }

        } while (!WLLERR);

        switch (wallType)
        {
        case 'v':
        case 'V':
            vWall[wallsPosition[i][0]][wallsPosition[i][1]] = 1;
            break;
        case 'h':
        case 'H':
            hWall[wallsPosition[i][0]][wallsPosition[i][1]] = 1;
            break;
        default:
            printf("Error: wall type is incurrect\n");
            i--;
        }
    }

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            map[i][j] = ' ';
        }
    }

    map[lightCorePosition[0]][lightCorePosition[1]] = 'C';

    for (i = 0; i < playerCount; i++)
    {
        map[playersPosition[i][0]][playersPosition[i][1]] = 'P';
    }

    for (i = 0; i < hunterCount; i++)
    {
        map[huntersPosition[i][0]][huntersPosition[i][1]] = 'H';
    }

    clearScreen();

    // ----- information -----
    // printf("CORE POSITION : %d - %d\n", lightCorePosition[0], lightCorePosition[1]);
    // for (i = 0; i < playerCount; i++)
    // {
    //     printf("PLAYER NO %d POSITION : %d - %d\n", i + 1, playersPosition[i][0], playersPosition[i][1]);
    // }
    // for (i = 0; i < hunterCount; i++)
    // {
    //     printf("HUNTER NO %d POSITION : %d - %d\n", i + 1, huntersPosition[i][0], huntersPosition[i][1]);
    // }

    for (i = 0; i < n; i++)
    {
        // ----- Top walls -----
        for (j = 0; j < m; j++)
        {
            printf("*");
            if (i == 0 || vWall[i - 1][j])
                printf("---");
            else
                printf("   ");
        }
        printf("*\n");

        // ----- Cells + vertical walls -----
        for (j = 0; j < m; j++)
        {
            if (j == 0 || hWall[i][j - 1])
                printf("|");
            else
                printf(" ");

            printf(" %c ", map[i][j]);
        }
        printf("|\n");
    }

    // ----- Bottom walls -----

    for (j = 0; j < m; j++)
    {
        printf("*---");
    }
    printf("*\n");

    return 0;
}
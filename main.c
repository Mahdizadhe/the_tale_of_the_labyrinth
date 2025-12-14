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
    printf("Please enter n, m, player count, player position, hunter count, hunter position, wall count, wall position");
    int n, m, playerCount, hunterCount, wallsCount;

    char map[100][100];

    int lightCorePosition[2];   // 1 col for light core and 2 row for X and Y
    int playersPosition[10][2]; // 10 col for playerCount and 2 row for X and Y
    int huntersPosition[10][2]; // 10 col for hunterCount and 2 row for X and Y
    int wallsPosition[100][2];  // 100 col for wallCount and 1 row X and 1 row for Y

    int vWall[100][100] = {0}; // this array is for walls between (x) and (x + 1)
    int hWall[100][100] = {0}; // this array is for walls between (y) nad (y + 1)

    int i, j;

    scanf("%d %d", &n, &m);
    scanf("%d %d", &lightCorePosition[0], &lightCorePosition[1]);

    scanf("%d", &playerCount);
    for (i = 0; i < playerCount; i++)
    {
        scanf("%d %d", &playersPosition[i][0], &playersPosition[i][1]);
    }

    scanf("%d", &hunterCount);
    for (i = 0; i < hunterCount; i++)
    {
        scanf("%d %d", &huntersPosition[i][0], &huntersPosition[i][1]);
    }

    scanf("%d", &wallsCount);
    for (i = 0; i < wallsCount; i++)
    {
        char wallType;
        scanf("%d %d %c", &wallsPosition[i][0], &wallsPosition[i][1], &wallType);
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

    for (i = 0; i < n; i++)
    {
        // ----- Top walls -----
        for (j = 0; j < m; j++)
        {
            printf("+");
            if (i == 0 || vWall[i - 1][j])
                printf("---");
            else
                printf("   ");
        }
        printf("+\n");

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
        printf("+---");
    }
    printf("+\n");

    return 0;
}
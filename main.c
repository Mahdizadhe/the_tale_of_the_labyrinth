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
    int n, m, playerCount, hunterCount, wallsCount;

    char map[100][100];

    int lightCorePosition[2];   // 1 col for light core and 2 row for X and Y
    int playersPosition[10][2]; // 10 col for playerCount and 2 row for X and Y
    int huntersPosition[10][2]; // 10 col for hunterCount and 2 row for X and Y
    int wallsPosition[100][3];  // 100 col for wallCount and 1 row X and 1 row for Y and 1 row for position (H or V | 1 => H and 2 => V)

    int i, j;

    scanf("%d %d", &n, &m);
    scanf("%d %d", &lightCorePosition[0], &lightCorePosition[1]);
    scanf("%d", &playerCount);

    for (i = 0; i < playerCount; i++)
    {
        scanf("%d", &playersPosition[i][0]);
        scanf("%d", &playersPosition[i][1]);
    }

    scanf("%d", &hunterCount);
    for (i = 0; i < hunterCount; i++)
    {
        scanf("%d %d", &huntersPosition[i][0], &huntersPosition[i][1]);
    }

    char wallType;

    scanf("%d", &wallsCount);
    for (i = 0; i < wallsCount; i++)
    {
        scanf("%d %d %c", &wallsPosition[i][0], &wallsPosition[i][1], &wallType);
        switch (wallType)
        {
        case 'v':
        case 'V':
            wallsPosition[i][2] = '2';
            break;
        case 'h':
        case 'H':
            wallsPosition[i][2] = '1';
            break;
        default:
            printf("WALL TYPE ERROR\n");
        }
    }

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            map[i][j] = '.';
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
        for (j = 0; j < m; j++)
        {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }

    return 0;
}
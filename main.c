#include <stdio.h>
// #include <windows.h>
// #include <conio.h>
#include <time.h>
// #include <ctime>

void clearScreen()
{
    system("cls");
}

int main()
{
    int boardGame[12][12], n, m, lightCorePositionX, lightCorePositionY, playerNo, playerPositionX, playerPositionY, hunterNo, hunterPositionX, hunterPositionY, wallsNo, wallsPositionX, wallsPositionY;

    int lightCorePosition[2];     // 4 col for playerNo and 2 col for X and Y
    int playersPosition[10][2];   // 10 col for playerNo and 2 col for X and Y
    int huntersPosition[10][2];   // 10 col for HunterNo and 2 col for X and Y
    int wallsPosition[12][12][1]; // 12 col for X and 12 col for Y and 1 col for wall position (H or V | 0 => H and 1 => V)

    scanf("%d %d", &n, &m);
    scanf("%d %d", &lightCorePosition[0], &lightCorePosition[1]);
    scanf("%d", &playerNo);

    for (int i = 0; i < playerNo; i++)
    {
        scanf("%d", &playersPosition[i][0]);
        scanf("%d", &playersPosition[i][1]);
    }

    scanf("%d", &hunterNo);
    for (int i = 0; i < hunterNo; i++)
    {
        scanf("%d %d", &huntersPosition[i][0], &huntersPosition[i][1]);
    }

    scanf("%d", &wallsNo);
    for (int i = 0; i < wallsNo; i++)
    {
        // scanf("%d %d %d", &wallsPosition[i][0]);
        // scanf("%d %d %d", &wallsPosition[i][1]);
        // -------------- wrong --------------
    }

    return 0;
}
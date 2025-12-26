#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
// #include <ctime.h>

//====================/ Global Vars /====================//

int n, m, playerCount, hunterCount, wallCount;
char map[100][100];

int lightCorePosition[2];   // 1 col for light core and 2 row for X and Y
int playersPosition[10][2]; // 10 col for playerCount and 2 row for X and Y
int huntersPosition[10][2]; // 10 col for hunterCount and 2 row for X and Y

int vWall[100][100] = {0}; // this array is for walls between (x) and (x + 1)
int hWall[100][100] = {0}; // this array is for walls between (y) and (y + 1)

int visited[100][100]; // this array is for checking connection of cells
int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

int i, j;

//====================/ Functions /====================//

void clearScreen()
{
    system("cls");
}

int inRange(int x, int y)
{
    return (x >= 0 && x < n && y >= 0 && y < m);
}

// isBlocked() = 0 -> There is no wall
// isBlocked() = 1 -> There is a wall
int isBlocked(int x, int y, int nx, int ny)
{
    if (x == nx)
    {
        if (ny > y)
        {
            return hWall[x][y];
        }
        else
        {
            return hWall[x][ny];
        }
    }
    if (y == ny)
    {
        if (nx > x)
        {
            return vWall[x][y];
        }
        else
        {
            return vWall[nx][y];
        }
    }
    return 0;
}

void bfs(int sx, int sy)
{
    int qx[100 * 100], qy[100 * 100];
    int front = 0, back = 0;

    qx[back] = sx;
    qy[back++] = sy;
    visited[sx][sy] = 1;

    while (front < back)
    {
        int x = qx[front];
        int y = qy[front++];
        for (i = 0; i < 4; i++)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (inRange(nx, ny) && !visited[nx][ny] && !isBlocked(x, y, nx, ny))
            {
                visited[nx][ny] = 1;
                qx[back] = nx;
                qy[back++] = ny;
            }
        }
    }
}

int isConnected()
{

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            visited[i][j] = 0;
        }
    }

    bfs(0, 0);
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            if (!visited[i][j])
            {
                return 0;
            }
        }
    }
    return 1;
}

//====================/ Walls Generation /====================//

void generateWalls(int wallCount)
{

    for (i = 0; i < wallCount; i++)
    {
        int x, y, type, error;
        do
        {
            error = 0;
            type = rand() % 2; // 0 -> V , 1 -> H
            if (type == 0)
            {
                x = rand() % (n - 1);
                y = rand() % m;

                if (vWall[x][y])
                {
                    error = 1;
                }
            }
            else
            {
                x = rand() % n;
                y = rand() % (m - 1);

                if (hWall[x][y])
                {
                    error = 1;
                }
            }
        } while (error);

        if (type == 0)
        {
            vWall[x][y] = 1;
        }
        else
        {
            hWall[x][y] = 1;
        }
    }
}

int distance(int x1, int y1, int x2, int y2)
{
    int xDistance = x1 - x2;
    if (xDistance < 0)
    {
        xDistance *= (-1);
    }

    int yDistance = y1 - y2;
    if (yDistance < 0)
    {
        yDistance *= (-1);
    }

    return xDistance + yDistance;
}
//====================/ Core, Players, Hunters /====================//
void placeCore()
{
    lightCorePosition[0] = rand() % n;
    lightCorePosition[1] = rand() % m;
}

void placePlayers()
{
    for (i = 0; i < playerCount; i++)
    {
        int x, y, error;
        do
        {
            error = 0;
            x = rand() % n;
            y = rand() % m;

            if (distance(x, y, lightCorePosition[0], lightCorePosition[1]) < 2)
            {
                error = 1;
            }

            for (j = 0; j < i; j++)
            {
                // if (distance(x, y, playersPosition[j][0], playersPosition[j][1]) < 2)
                if (x == playersPosition[j][0] && y == playersPosition[j][1])
                {
                    error = 1;
                }
            }

        } while (error);

        playersPosition[i][0] = x;
        playersPosition[i][1] = y;
    }
}

void placeHunters()
{
    for (i = 0; i < hunterCount; i++)
    {
        int x, y, error;
        do
        {
            error = 0;
            x = rand() % n;
            y = rand() % m;

            if (distance(x, y, lightCorePosition[0], lightCorePosition[1]) < 2)
            {
                error = 1;
            }

            for (j = 0; j < playerCount; j++)
            {
                if (distance(x, y, playersPosition[j][0], playersPosition[j][1]) < 2)
                {
                    error = 1;
                }
            }

            for (j = 0; j < i; j++)
            {
                // if (distance(x, y, huntersPosition[j][0], huntersPosition[j][1]) < 2)
                if (x == huntersPosition[j][0] && y == huntersPosition[j][1])
                {
                    error = 1;
                }
            }

        } while (error);
        huntersPosition[i][0] = x;
        huntersPosition[i][1] = y;
    }
}

//====================/ Map Init /====================//

void initMap()
{
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
}

//====================/ Map Printing /====================//

void printMap()
{
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
}

//====================/ Main /====================//

int main()
{
    srand(time(NULL));

    // printf("Please enter n, m, players count, players position, hunters count, hunters position, walls count, walls position\n");
    printf("Please enter n, m, player count, hunter count, wall count\n");

    scanf("%d %d", &n, &m);
    scanf("%d", &playerCount);
    scanf("%d", &hunterCount);
    scanf("%d", &wallCount);

    //====================/ Core /====================//

    placeCore();

    //====================/ Players /====================//

    placePlayers();

    //====================/ Hunters /====================//

    placeHunters();

    //====================/ Walls /====================//

    do
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < m; j++)
            {
                vWall[i][j] = hWall[i][j] = 0;
            }
        }
        generateWalls(wallCount); // there is a bug
    } while (!isConnected());

    //====================/ Map /====================//

    initMap();
    clearScreen();

    //====================/ information /====================//

    printf("CORE POSITION : [%d - %d]\n", lightCorePosition[0], lightCorePosition[1]);
    printf("*******************************\n");
    for (i = 0; i < playerCount; i++)
    {
        printf("PLAYER NO. %d POSITION : [%d - %d]\n", i + 1, playersPosition[i][0], playersPosition[i][1]);
    }
    printf("*******************************\n");
    for (i = 0; i < hunterCount; i++)
    {
        printf("HUNTER NO. %d POSITION : [%d - %d]\n", i + 1, huntersPosition[i][0], huntersPosition[i][1]);
    }
    printf("*******************************\n\n");

    printMap();
    return 0;
}
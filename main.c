#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

//====================/ Struct /====================//

struct Entity
{
    int x;
    int y;
    int alive;
    int tempWallCount;
};

//====================/ Global Vars /====================//

int n, m, playerCount, hunterCount, wallCount;
char map[100][100];

struct Entity core;
struct Entity players[10];
struct Entity hunters[10];

int vWall[100][100] = {0}; // this array is for walls between (x) and (x + 1)
int hWall[100][100] = {0}; // this array is for walls between (y) and (y + 1)

int vTempWall[100][100] = {0};
int hTempWall[100][100] = {0};

int vTempTime[100][100];
int hTempTime[100][100];

int visited[100][100]; // this array is for checking connection of cells
int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

int i, j;

//====================/ Functions /====================//

void clearScreen()
{
    system("cls");
}

// inRange() = 0 -> Not within range.
// inRange() = 1 -> It is within range.
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
            return hWall[x][y] || hTempWall[x][y];
        }
        else
        {
            return hWall[x][ny] || hTempWall[x][ny];
        }
    }
    if (y == ny)
    {
        if (nx > x)
        {
            return vWall[x][y] || vTempWall[x][y];
        }
        else
        {
            return vWall[nx][y] || vTempWall[nx][y];
        }
    }
    return 0;
}

void bfs(int sx, int sy)
{
    int qx[10000], qy[10000];
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
    core.x = rand() % n;
    core.y = rand() % m;
}

void placePlayers()
{
    for (i = 0; i < playerCount; i++)
    {
        players[i].alive = 1;
        players[i].tempWallCount = max(min(n, m) / 3, 1);
        int x, y, error;
        do
        {
            error = 0;
            x = rand() % n;
            y = rand() % m;

            if (distance(x, y, core.x, core.y) < 2)
            {
                error = 1;
            }

            for (j = 0; j < i; j++)
            {
                if (x == players[j].x && y == players[j].y)
                {
                    error = 1;
                }
            }

        } while (error);

        players[i].x = x;
        players[i].y = y;
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

            if (distance(x, y, core.x, core.y) < 2)
            {
                error = 1;
            }

            for (j = 0; j < playerCount; j++)
            {
                if (distance(x, y, players[j].x, players[j].y) < 2)
                {
                    error = 1;
                }
            }

            for (j = 0; j < i; j++)
            {
                if (x == hunters[j].x && y == hunters[j].y)
                {
                    error = 1;
                }
            }

        } while (error);
        hunters[i].x = x;
        hunters[i].y = y;
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

    map[core.x][core.y] = 'C';

    for (i = 0; i < playerCount; i++)
    {
        if (players[i].alive)
            map[players[i].x][players[i].y] = 'P';
    }

    for (i = 0; i < hunterCount; i++)
    {
        map[hunters[i].x][hunters[i].y] = 'H';
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
            else if (vTempWall[i - 1][j])
                printf("~~~");
            else
                printf("   ");
        }
        printf("*\n");

        // ----- Cells + vertical walls -----
        for (j = 0; j < m; j++)
        {
            if (j == 0 || hWall[i][j - 1])
                printf("|");
            else if (hTempWall[i][j - 1])
                printf("/");
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

//====================/ Movement /====================//

int nearestHunterDistance(int x, int y)
{
    int minDist = distance(x, y, hunters[0].x, hunters[0].y);
    for (int i = 1; i < hunterCount; i++)
    {
        int d = distance(x, y, hunters[i].x, hunters[i].y);
        if (d < minDist)
            minDist = d;
    }
    return minDist;
}

int suggestBestMove(int idx)
{
    int x = players[idx].x;
    int y = players[idx].y;

    int bestMove = 4; // STAY
    int bestScore = -100000;

    for (int dir = 0; dir < 5; dir++)
    {
        int nx = x;
        int ny = y;

        if (dir == 0)
            nx--; // UP
        else if (dir == 1)
            nx++; // DOWN
        else if (dir == 2)
            ny--; // LEFT
        else if (dir == 3)
            ny++; // RIGHT
        // dir == 4 => STAY

        if (dir != 4)
        {
            if (!inRange(nx, ny))
                continue;
            if (isBlocked(x, y, nx, ny))
                continue;
        }

        int score = 0;

        score += (distance(x, y, core.x, core.y) - distance(nx, ny, core.x, core.y)) * 10;

        int oldHunterDist = nearestHunterDistance(x, y);
        int newHunterDist = nearestHunterDistance(nx, ny);
        score += (newHunterDist - oldHunterDist) * 20;

        if (newHunterDist <= 1)
        {
            score -= 1000;
        }

        if (score > bestScore)
        {
            bestScore = score;
            bestMove = dir;
        }
    }

    return bestMove;
}

int findNearestPlayer(int idx)
{
    int hx = hunters[idx].x;
    int hy = hunters[idx].y;
    int bestIdx = -1;
    int bestDist = 100000;

    for (i = 0; i < playerCount; i++)
    {
        if (!players[i].alive)
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

int movePlayer(int idx)
{
    int key;
    int x = players[idx].x;
    int y = players[idx].y;

    while (1)
    {
        key = getch();
        if (key == 224)
            key = getch();

        int nx = x;
        int ny = y;

        if (key == 72) // UP
            nx--;
        else if (key == 80) // DOWN
            nx++;
        else if (key == 75) // LEFT
            ny--;
        else if (key == 77) // RIGHT
            ny++;
        else if (key == 'S' || key == 's')
            break;
        else if (key == 'V' || key == 'v')
        {
            if (y >= m - 1 || vWall[x][y] || vTempWall[x][y])
            {
                printf("Cannot place a vertical wall\n");
                continue;
            }
            vTempWall[x][y] = 1;
            vTempTime[x][y] = 2;

            break;
        }
        else if (key == 'H' || key == 'h')
        {
            if (x >= n - 1 || hWall[x][y] || hTempWall[x][y])
            {
                printf("Cannot place a horizonetal wall\n");
                continue;
            }
            hTempWall[x][y] = 1;
            hTempTime[x][y] = 2;

            break;
        }
        else
        {
            printf("Move error: Invalid move\n");
            continue;
        }

        if (!inRange(nx, ny))
        {
            printf("Move error: Out of map\n");
            continue;
        }

        if (isBlocked(x, y, nx, ny))
        {
            printf("Move error: blocked by wall\n");
            continue;
        }

        players[idx].x = nx;
        players[idx].y = ny;
        break;
    }

    return 1;
}

void updateTempWall()
{
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            if (vTempWall[i][j])
            {
                vTempTime[i][j]--;
                if (vTempTime[i][j] == 0)
                    vTempWall[i][j] = 0;
            }
            if (hTempWall[i][j])
            {
                hTempTime[i][j]--;
                if (hTempTime[i][j] == 0)
                    hTempWall[i][j] = 0;
            }
        }
    }
}

int moveHunter(int idx)
{
    int hx = hunters[idx].x;
    int hy = hunters[idx].y;

    int nearestPlayer = findNearestPlayer(idx);
    if (nearestPlayer == -1)
        return 0;
    int px = players[nearestPlayer].x;
    int py = players[nearestPlayer].y;
    int dist = distance(hx, hy, px, py);

    int bestX = hx;
    int bestY = hy;
    int bestDist = dist;

    if (hy < py)
    {
        int nx = hx;
        int ny = hy + 1;
        if (inRange(nx, ny) && !isBlocked(hx, hy, nx, ny))
        {
            int d = distance(nx, ny, px, py);
            if (d < bestDist)
            {
                bestDist = d;
                bestX = nx;
                bestY = ny;
            }
        }
    }
    else if (hy > py)
    {
        int nx = hx;
        int ny = hy - 1;
        if (inRange(nx, ny) && !isBlocked(hx, hy, nx, ny))
        {
            int d = distance(nx, ny, px, py);
            if (d < bestDist)
            {
                bestDist = d;
                bestX = nx;
                bestY = ny;
            }
        }
    }

    if (hx < px)
    {
        int nx = hx + 1;
        int ny = hy;
        if (inRange(nx, ny) && !isBlocked(hx, hy, nx, ny))
        {
            int d = distance(nx, ny, px, py);
            if (d < bestDist)
            {
                bestDist = d;
                bestX = nx;
                bestY = ny;
            }
        }
    }
    else if (hx > px)
    {
        int nx = hx - 1;
        int ny = hy;
        if (inRange(nx, ny) && !isBlocked(hx, hy, nx, ny))
        {
            int d = distance(nx, ny, px, py);
            if (d < bestDist)
            {
                bestDist = d;
                bestX = nx;
                bestY = ny;
            }
        }
    }
    hunters[idx].x = bestX;
    hunters[idx].y = bestY;
    return 1;
}

//====================/ Game /====================//
// gameState() = 0 -> resume
// gameState() = 1 -> lose game
// gameState() = 2 -> won game
int gameState()
{
    int aliveCount = 0;

    for (i = 0; i < hunterCount; i++)
    {
        for (j = 0; j < playerCount; j++)
        {
            if (players[j].alive && hunters[i].x == players[j].x && hunters[i].y == players[j].y)
            {
                players[j].alive = 0;
            }
        }
    }

    for (i = 0; i < playerCount; i++)
    {
        if (players[i].alive)
            aliveCount++;
    }

    if (aliveCount == 0)
    {
        return 1;
    }

    for (i = 0; i < playerCount; i++)
    {
        if (players[i].alive && players[i].x == core.x && players[i].y == core.y)
        {
            return 2;
        }
    }

    return 0;
}

void game()
{
    while (1)
    {
        //====================/ init map /====================//

        initMap();
        clearScreen();

        //====================/ information /====================//

        printf("CORE POSITION : [%d - %d]\n", core.x, core.y);
        printf("*******************************\n");
        for (i = 0; i < playerCount; i++)
        {
            printf("PLAYER NO. %d POSITION : [%d - %d]\n", i + 1, players[i].x, players[i].y);
        }
        printf("*******************************\n");
        for (i = 0; i < hunterCount; i++)
        {
            printf("HUNTER NO. %d POSITION : [%d - %d]\n", i + 1, hunters[i].x, hunters[i].y);
        }
        printf("*******************************\n\n");

        //====================/ print map /====================//

        printMap();

        updateTempWall();

        //====================/ movement /====================//

        for (i = 0; i < playerCount; i++)
        {

            if (!players[i].alive)
                continue;

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

            movePlayer(i);

            if (gameState() != 0)
                return;
        }

        for (i = 0; i < hunterCount; i++)
        {
            moveHunter(i);
        }
        if (gameState() != 0)
            return;
    }
}

//====================/ Main /====================//

int main()
{
    srand(time(NULL));

    int code;

    while (1)
    {
        clearScreen();

        printf("THE TALE OF THE LABYRINTH\nPlease enter any code:\n");
        printf("1. START GAME\n");
        printf("2. CONTINUE\n");
        printf("3. HOW TO PLAY\n");
        printf("4. CREDITS\n");
        printf("5. EXIT\n");

        scanf("%d", &code);
        if (code == 1)
        {
            clearScreen();
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
                generateWalls(wallCount);
            } while (!isConnected());

            //====================/ Game /====================//

            game();

            clearScreen();
            if (gameState() == 1)
            {
                printf("YOU LOSE\n");
            }
            else
            {
                printf("YOU WON\n");
            }
            printf("Press eny key to back to menu\n");
            getch();
        }
        else if (code == 2)
        {
            clearScreen();
            printf("Continue is not implemented yet!\n");
            printf("Press eny key to back to menu\n");
            getch();
        }
        else if (code == 3)
        {
            clearScreen();
            printf("Use arrow keys to move\n");
            printf("Use 's' to delete turn (not moving)\n");
            printf("Press eny key to back to menu\n");
            getch();
        }
        else if (code == 4)
        {
            clearScreen();
            printf("STUDENT: MOHAMMAD MAHDIZADHE\n");
            printf("STUDENT ID: 4042262277\n");
            printf("Press eny key to back to menu\n");
            getch();
        }
        else if (code == 5)
        {
            break;
        }
    }

    return 0;
}
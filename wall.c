#include "wall.h"
#include "map.h"
#include <stdlib.h>

//====================/ Global Variables /====================//

int wallCount; // Total number of walls to generate

// Vertical -> عمودی
// Vertical walls: vWall[x][y] represents wall between (x, y) and (x+1, y)
int vWall[100][100];

// Horizontal -> افقی
// Horizontal walls: hWall[x][y] represents wall between (x, y) and (x, y+1)
int hWall[100][100];

// Temporary walls (placed by players)
int vTempWall[100][100];
int hTempWall[100][100];

// Duration timers for temporary walls
int vTempTime[100][100];
int hTempTime[100][100];

// Visited array for BFS during connectivity check
int visited[100][100];

// Directions for BFS: up, down, left, right
int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

//====================/ Utility Functions /====================//

// Check if (x, y) is inside the map
// Returns 1 if in range, 0 otherwise
int inRange(int x, int y)
{
    if (x >= 0 && x < n && y >= 0 && y < m)
        return 1;
    else
        return 0;
}

// Check if movement from (x, y) to (nx, ny) is blocked by a wall
// Returns 1 if blocked, 0 if free
int isBlocked(int x, int y, int nx, int ny)
{
    if (x == nx) // Moving vertically
    {
        if (ny > y)
            return hWall[x][y] || hTempWall[x][y]; // wall below current
        else
            return hWall[x][ny] || hTempWall[x][ny]; // wall above next
    }
    if (y == ny) // Moving horizontally
    {
        if (nx > x)
            return vWall[x][y] || vTempWall[x][y]; // wall to the right
        else
            return vWall[nx][y] || vTempWall[nx][y]; // wall to the left
    }
}

//====================/ BFS for Connectivity /====================//

// BFS to mark all reachable cells from (sx, sy)
void bfs(int sx, int sy)
{
    int qx[10000], qy[10000]; // BFS queue
    int front = 0, back = 0;

    qx[back] = sx;
    qy[back++] = sy;
    visited[sx][sy] = 1;

    while (front < back)
    {
        int x = qx[front];
        int y = qy[front++];
        for (int i = 0; i < 4; i++)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (inRange(nx, ny) && !visited[nx][ny] && !isBlocked(x, y, nx, ny))
            {
                visited[nx][ny] = 1; // Mark as reachable
                qx[back] = nx;
                qy[back++] = ny; // Add to queue
            }
        }
    }
}

// Check if the map is fully connected (no isolated sections)
int isConnected()
{
    // Reset visited array
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            visited[i][j] = 0;

    bfs(0, 0); // Start BFS from [0-0]

    // If any cell is not visited, map is disconnected
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (!visited[i][j])
                return 0;

    return 1; // All cells are reachable
}

//====================/ Walls Generation /====================//

// Randomly generate vertical and horizontal walls on the map
void generateWalls(int wallCount)
{
    for (int i = 0; i < wallCount; i++)
    {
        int x, y, type, error;

        // Repeat until a valid wall position is found
        do
        {
            error = 0;
            type = rand() % 2; // 0 = vertical, 1 = horizontal

            if (type == 0) // Vertical wall
            {
                x = rand() % (n - 1);
                y = rand() % m;
                if (vWall[x][y])
                    error = 1; // Already a wall here
            }
            else // Horizontal wall
            {
                x = rand() % n;
                y = rand() % (m - 1);
                if (hWall[x][y])
                    error = 1; // Already a wall here
            }
        } while (error);

        // Place the wall
        if (type == 0)
            vWall[x][y] = 1;
        else
            hWall[x][y] = 1;
    }
}

//====================/ Temporary Wall Updates /====================//

// Reduce duration of temporary walls and remove expired ones
void updateTempWall()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (vTempWall[i][j])
            {
                vTempTime[i][j]--;
                if (vTempTime[i][j] == 0)
                    vTempWall[i][j] = 0; // Remove expired wall
            }
            if (hTempWall[i][j])
            {
                hTempTime[i][j]--;
                if (hTempTime[i][j] == 0)
                    hTempWall[i][j] = 0; // Remove expired wall
            }
        }
    }
}
#ifndef WALL_H
#define WALL_H

extern int wallCount;

extern int hWall[100][100];
extern int vWall[100][100];

extern int hTempWall[100][100];
extern int vTempWall[100][100];

extern int hTempTime[100][100];
extern int vTempTime[100][100];

extern int visited[100][100]; // this array is for checking connection of cells
extern int dx[4];
extern int dy[4];

int inRange(int x, int y);
int isBlocked(int x, int y, int nx, int ny);
void generateWalls(int wallCount);
int isConnected();
void updateTempWall();

#endif

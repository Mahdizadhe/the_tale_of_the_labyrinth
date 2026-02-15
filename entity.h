#ifndef ENTITY_H
#define ENTITY_H

struct Entity
{
    int x;
    int y;
    int alive;
    int tempWallCount;
};

extern int playerCount;
extern int hunterCount;

extern struct Entity core;
extern struct Entity players[10];
extern struct Entity hunters[10];

void placeCore();
void placePlayers();
void placeHunters();

int distance(int x1, int y1, int x2, int y2);

#endif

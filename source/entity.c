#include <nds.h>
#include <stdio.h>
#include <nf_lib.h>

typedef struct Sprite
{
    int screen;
    int id;
    int gfx;
    int pal;
    int x;
    int y;
    int isFlipped;
    int velocityX;
    int velocityY;
    int shouldFall;
} Sprite;
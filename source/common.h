#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <switch.h>

#define BLACK RGBA8_MAXALPHA(0,0,0)
#define BLUE  RGBA8_MAXALPHA(0,0,255)
#define AQUA  RGBA8_MAXALPHA(0,255,255)
#define RED   RGBA8_MAXALPHA(255,0,0)

typedef struct gameCtx {
    bool inGame;
    bool splash;
    int level;
    int xPos;
    int yPos;
} gameCtx;

void drawCursor(gameCtx *ctx);
void drawLevel1(gameCtx *ctx);
void drawLevel2(gameCtx *ctx);
void drawLevel3(gameCtx *ctx);
void keepInBounds(gameCtx *ctx);
void checkToScream(gameCtx *ctx);
void reset(gameCtx *ctx);
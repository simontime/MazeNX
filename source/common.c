#include "gfx.h"
#include "audio.h"

#define DRAWPATH(x, y, w, h) drawRectangle(ctx, false, x, y, w, h, AQUA);
#define LEVELEND(x, y, w, h) drawRectangle(ctx, false, x, y, w, h, RED);

// Draws the cursor, then draws the framebuffer to screen.
void drawCursor(gameCtx *ctx) {
    drawRectangle(ctx, true, ctx->xPos, ctx->yPos, 16, 16, BLUE);
}

// Draws level 1.
void drawLevel1(gameCtx *ctx) {
    DRAWPATH(480,  0, 320, 720);
    DRAWPATH(800,  0, 340, 60);
    LEVELEND(1140, 0, 140, 60);
}

void drawLevel2(gameCtx *ctx) {
    DRAWPATH(80,   0,   1120, 70);
    DRAWPATH(80,   70,  70,   100);
    DRAWPATH(80,   170, 1120, 70);
    DRAWPATH(1130, 240, 70,   100);
    DRAWPATH(80,   340, 1120, 70);
    DRAWPATH(80,   410, 70,   100); 
    DRAWPATH(80,   510, 1120, 70);
    DRAWPATH(1030, 580, 170,  100);
    DRAWPATH(150,  680, 1050, 50);
    LEVELEND(80,   680, 70,   50);
}

void drawLevel3(gameCtx *ctx) {
    DRAWPATH(80,   660, 1120, 60);
    DRAWPATH(1140, 600, 60,   60);
    DRAWPATH(80,   540, 1120, 60);
    DRAWPATH(80,   380, 60,   160);
    DRAWPATH(80,   320, 560,  60);
    DRAWPATH(640,  320, 50,   20);
    DRAWPATH(670,  240, 20,   80);
    DRAWPATH(620,  220, 70,   20);
    DRAWPATH(620,  180, 20,   50);
    DRAWPATH(620,  160, 70,   20);
    DRAWPATH(670,  120, 20,   50);
    LEVELEND(630,  0,   100,  120);
}

// Ensures the cursor never travels out of bounds (console will fatal) and acts as a collision check.
void keepInBounds(gameCtx *ctx) {
    if (ctx->xPos > 1264 || ctx->yPos > 704 || 
        ctx->xPos < 0    || ctx->yPos < 0)
        reset(ctx);
}

void checkToScream(gameCtx *ctx) {
    if (ctx->level == 3 && ctx->yPos < 200) {
        renderImage(false);
        updateDisplay();
        playScream();
        playScream();
        reset(ctx);
    }
}

void reset(gameCtx *ctx) {
    ctx->inGame = false;
    ctx->splash = false;
    ctx->level  = 0;
    ctx->xPos   = 632;
    ctx->yPos   = 632;
}
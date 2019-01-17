#include "gfx.h"
#include "audio.h"

static gameCtx ctx = {
    .inGame = false,
    .splash = false,
    .level  = 0,
    .xPos   = 632,
    .yPos   = 632
};

int main(int argc, char **argv) {
    romfsInit();

    initGfx();
    initAudio();

    loadSplash();
    loadFace();
    loadAudio();

    while (appletMainLoop()) {
        hidScanInput();

        u64 kDown = hidKeysHeld(CONTROLLER_P1_AUTO),
            boost = kDown & KEY_ZR;

        if (!ctx.inGame && !ctx.splash) 
            renderImage(true);

        if (kDown & KEY_PLUS && !ctx.inGame) 
            break;

        if (kDown & KEY_A && !ctx.inGame) {
            ctx.inGame = true;
            ctx.level++;
        };

        if (ctx.inGame && kDown & KEY_LEFT)
            ctx.xPos -= boost ? 6 : 2;
        if (ctx.inGame && kDown & KEY_RIGHT)
            ctx.xPos += boost ? 6 : 2;
        if (ctx.inGame && kDown & KEY_UP)
            ctx.yPos -= boost ? 6 : 2;
        if (ctx.inGame && kDown & KEY_DOWN)
            ctx.yPos += boost ? 6 : 2;
 
        keepInBounds(&ctx);
        checkToScream(&ctx);

        if (ctx.inGame && ctx.level == 1) {
            clearFramebuffer();
            drawLevel1(&ctx);
            drawCursor(&ctx);
        }

        if (ctx.inGame && ctx.level == 2) {
            clearFramebuffer();
            drawLevel2(&ctx);
            drawCursor(&ctx);
        }

        if (ctx.inGame && ctx.level == 3) {
            clearFramebuffer();
            drawLevel3(&ctx);
            drawCursor(&ctx);
        }
    }

    closeGfx();
    closeAudio();

    romfsExit();

    return 0;
}
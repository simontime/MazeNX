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

        touchPosition pos;
        u32 pid = 0;

        u64 kDown = hidKeysHeld(CONTROLLER_P1_AUTO),
            boost = kDown & KEY_ZR || hidTouchCount() > 1 ? 1 : 0;

        hidTouchRead(&pos, pid);

        if (!ctx.inGame && !ctx.splash) 
            renderImage(true);

        if (kDown & KEY_PLUS && !ctx.inGame) 
            break;

        if ((kDown & KEY_A || kDown & KEY_TOUCH) && !ctx.inGame) {
            ctx.inGame = true;
            ctx.level++;
        };

        if (ctx.inGame && (kDown & KEY_LEFT  || (pos.px > 0 && pos.px < 320)))
            ctx.xPos -= boost ? 6 : 2;
        if (ctx.inGame && (kDown & KEY_RIGHT ||  pos.px > 960))
            ctx.xPos += boost ? 6 : 2;
        if (ctx.inGame && (kDown & KEY_UP    || (pos.py > 0 && pos.py < 220)))
            ctx.yPos -= boost ? 6 : 2;
        if (ctx.inGame && (kDown & KEY_DOWN  ||  pos.py > 500))
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

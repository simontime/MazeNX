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

        MousePosition mpos;
        touchPosition tpos;
        u32 pid = 0;

        u64 kDown = hidKeysHeld(CONTROLLER_P1_AUTO),
            mDown = hidMouseButtonsDown(),
            boost = kDown & KEY_ZR || hidTouchCount() > 1 ? 1 : 0;

        hidMouseRead(&mpos);
        hidTouchRead(&tpos, pid);

        if (!ctx.inGame && !ctx.splash) 
            renderImage(true);

        if (kDown & KEY_PLUS && !ctx.inGame) 
            break;

        if ((kDown & KEY_A || kDown & KEY_TOUCH || mDown & MOUSE_LEFT) && !ctx.inGame) {
            ctx.inGame = true;
            ctx.level++;
        };

        if (mpos.x && mpos.y) {
            ctx.xPos = mpos.x;
            ctx.yPos = mpos.y;
            if (!ctx.inGame)
                drawRectangle(&ctx, false, ctx.xPos, ctx.yPos, 16, 16, BLUE);
            if (mpos.y <= 1)
                reset(&ctx);
        }

        if (ctx.inGame && (kDown & KEY_LEFT  || (tpos.px && tpos.px < 320)))
            ctx.xPos -= boost ? 6 : 2;
        if (ctx.inGame && (kDown & KEY_RIGHT ||  tpos.px > 960))
            ctx.xPos += boost ? 6 : 2;
        if (ctx.inGame && (kDown & KEY_UP    || (tpos.py && tpos.py < 220)))
            ctx.yPos -= boost ? 6 : 2;
        if (ctx.inGame && (kDown & KEY_DOWN  ||  tpos.py > 500))
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

        updateDisplay();
    }

    closeGfx();
    closeAudio();

    romfsExit();

    return 0;
}
#include "libs/nanojpeg.h"
#include "libs/tinf.h"

#include "gfx.h"

#define FBSIZE 1280 * 720 * 4

static Framebuffer fb;

static u32 stride,
        *framebuf,
  fbSize = FBSIZE;

static u8 tmpbuf[FBSIZE],
          splash[FBSIZE],
          scream[FBSIZE];

void initGfx() {
    framebufferCreate(&fb, nwindowGetDefault(), 1280, 720, PIXEL_FORMAT_RGBA_8888, 2);
    framebufferMakeLinear(&fb);
    framebuf = (u32 *)framebufferBegin(&fb, &stride);
    memset(framebuf, 0, stride * 720);
    framebufferEnd(&fb);
}

/*
    Reads in and decompresses the splash screen.
    The splash screen is a DEFLATE-compressed 1280x720 RGBA8888 framebuffer.
*/
void loadSplash() {
    FILE *file = fopen("romfs:/splash", "rb");

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);

    fread(tmpbuf, 1, size, file);
    fclose(file);

	tinf_init();
	tinf_uncompress(splash, &fbSize, tmpbuf, size);
}

// Reads in and decompresses the face JPEG.
void loadFace() {
    FILE *img = fopen("romfs:/face", "rb");

    fseek(img, 0, SEEK_END);
    int size = ftell(img);
    u8 *buf = malloc(size);
    rewind(img);
    fread(buf, 1, size, img);
    fclose(img);

	njInit();
    njDecode(buf, size);
    
    // RGB888 -> RGBA8888, fill each pixel with RGB image data + max alpha.
    for (int i = 0, j = 0; i < fbSize; i += 4, j += 3) {
        scream[i]     = njGetImage()[j];
        scream[i + 1] = njGetImage()[j + 1];
        scream[i + 2] = njGetImage()[j + 2];
        scream[i + 3] = 0xff;
    }

    njDone();
    free(buf);
}

void updateDisplay() {
    framebufferBegin(&fb, &stride);
    framebufferEnd(&fb);
}

/*
    Check collision.
    If the pixel of the framebuffer to be drawn is black (collision with walls), reset the game.
    If the pixel of the framebuffer to be drawn is red (collision with the level end marker), increment the level.
*/
static bool checkCollision(gameCtx *ctx, int x, int y) {
    if (framebuf[y * 1280 + x] == RED) {
        ctx->level++;
        return true;
    }

    if (framebuf[y * 1280 + x] == BLACK) {
        reset(ctx);
        return true;
    }

    return false;
}

void drawRectangle(gameCtx *ctx, bool collision, u32 x, u32 y, u32 w, u32 h, u32 colour) {
    for (u32 yPos = y; yPos < y + h; yPos++)
        for (u32 xPos = x; xPos < x + w; xPos++) {
            if (collision)
                if (checkCollision(ctx, xPos, yPos))
                    return;

            framebuf[yPos * 1280 + xPos] = colour;
        }
}

void renderImage(bool isSplash) {
    memcpy(framebuf, isSplash ? splash : scream, fbSize);
    updateDisplay();
}

void clearFramebuffer() {
    for (int i = 0; i < fbSize; i++)
        framebuf[i] = BLACK;
}

void closeGfx() {
    framebufferClose(&fb);
}

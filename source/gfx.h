#include "common.h"

void initGfx();
void loadSplash();
void loadFace();
void updateDisplay();
void drawRectangle(gameCtx *ctx, bool collision, u32 x, u32 y, u32 w, u32 h, u32 colour);
void renderImage(bool isSplash);
void clearFramebuffer();
void closeGfx();
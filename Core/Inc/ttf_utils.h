#ifndef __TTF_UTILS_H__
#define __TTF_UTILS_H__
#include "common.h"
/*
- x, y: upper left corner of string
- rect output Width and height contain rendered dimensions.
*/
extern void render_text(
    SDL_Renderer *renderer,
    int x,
    int y,
    const char *text,
    TTF_Font *font,
    SDL_Rect *rect,
    SDL_Color *color
);
#endif


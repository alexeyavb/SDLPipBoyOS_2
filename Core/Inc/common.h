#ifndef __COMMON_H_
#define __COMMON_H_
// std libs
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
// GL
#ifndef GLEW_STATIC
    #define GLEW_STATIC
#endif

#ifndef GLEX_STATIC
    #define GLEX_STATIC
#endif

#ifndef GL_STATIC
    #define GL_STATIC
#endif

#include <GL/gl.h>
#include <GL/glext.h>
// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
// 

// types
// typedef TTF_Font *PTTF_Font, **LPTTFFont;
// Arrays
#ifndef DEF_FONT_CACHE_SZ
    #define DEF_FONT_CACHE_SZ 16
#endif

extern TTF_Font* TTF_FontCache[DEF_FONT_CACHE_SZ];
extern SDL_Renderer *renderer;
extern SDL_Window *window;
#endif //__COMMON_H_

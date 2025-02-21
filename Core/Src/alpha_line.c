#include "common_defs.h"
#include "common.h"
#include "alpha_line.h"

#define DEF_ALPHA_LINE_WIDTH (int)(DEF_SCREEN_WIDTH)
#define DEF_ALPHA_LINE_HEIGHT (int)(DEF_SCREEN_HEIGHT/4)

// #define DEF_ALINE_DELAY 4000
// #define DEF_ALPHA_LINE_SCALE 0.1f

// #faf9fe
#define DEF_ALPHA_LINE_CLR_R 0xaa
#define DEF_ALPHA_LINE_CLR_G 0xa9
#define DEF_ALPHA_LINE_CLR_B 0xae
#define DEF_ALPHA_LINE_ACHANNEL 0x16
#define DEF_RECT_HEIGHT  (int)(((int) DEF_SCREEN_HEIGHT/3)/28)
extern SDL_Window *window;
extern SDL_Renderer *renderer;

// static bool thread_running = true;
static bool init_complete = false;
static int y = (-DEF_RECT_HEIGHT);

// static int longdelay;
// static int next_distortion;

static const int interval = 244;
static int current_time;



static int current_time = 0;
static SDL_Surface *surf; 
static SDL_Texture *tex; 
static SDL_Rect *rect;    

// SDL_Surface *surf; SDL_Texture *tex; 
// SDL_Rect rect = {0};

void init_surface(void){
    // SDL_Rect LineRect = {0,0, DEF_SCREEN_WIDTH, DEF_RECT_HEIGHT}; // x,y, w,hB
    // SDL_RenderFillRect(renderer, &LineRect);
    current_time = SDL_GetTicks();
    // next_distortion = SDL_GetTicks();
    rect = (SDL_Rect*)malloc(sizeof(struct SDL_Rect));
    
    // #afafaffa
    // static const SDL_Color white = {0xaf,0xaf,0xaf,0xfa};
    // static SDL_PixelFormat rgbpf;
    // rgbpf.format = 32; rgbpf.Amask = 0xaf; rgbpf.Gmask = 0xfa; rgbpf.Bmask = 0xfa; rgbpf.Rmask = 0xfa; 

    // #4fafaffa
    // surf = SDL_CreateRGBSurface(0, DEF_ALPHA_LINE_WIDTH, DEF_ALPHA_LINE_HEIGHT, 32, 0x00, 0x00, 0xaf, 0xaf);
    // SDL_SetSurfaceBlendMode(surf, SDL_BLENDMODE_BLEND);

    surf = SDL_CreateRGBSurface(0, DEF_ALPHA_LINE_WIDTH, DEF_ALPHA_LINE_HEIGHT, 32, 0x00, 0x00, 0xaa, 0xfa);
    rect->x = 0;
    rect->y = y;
    rect->w = surf->w;
    rect->h = surf->h;

    SDL_SetSurfaceBlendMode(surf, SDL_BLENDMODE_BLEND);
    
    // SDL_FillRect(surf, rect, SDL_MapRGBA(&rgbpf, white.r, white.g, white.b, white.a));

    tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_SetTextureAlphaMod(tex, DEF_ALPHA_LINE_ACHANNEL);
    // SDL_SetTextureAlphaMod(tex, 0xa8);
    init_complete = true;
}

void deinit_surface(void){
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
    free(rect);
    rect = NULL;
}

void draw_alpha_line_proc(void){
    // if(SDL_GetTicks() >= next_distortion)
    //     next_distortion = SDL_GetTicks() + rand() % 100 - 100;
    // else return;

    if(SDL_GetTicks() >= current_time){
        int delta = rand() % DEF_SCREEN_HEIGHT - DEF_ALPHA_LINE_HEIGHT;
        current_time = SDL_GetTicks() + interval;
        y = (DEF_SCREEN_HEIGHT >= y ) ? delta : (-DEF_RECT_HEIGHT);
    }
    // SDL_SetRenderDrawColor(renderer, DEF_ALPHA_LINE_CLR_R, DEF_ALPHA_LINE_CLR_G, DEF_ALPHA_LINE_CLR_B, DEF_ALPHA_LINE_ACHANNEL);
    rect->x = 0;
    rect->y = y;
    rect->w = surf->w;
    rect->h = surf->h;
    /* This is wasteful for textures that stay the same.
     * But makes things less stateful and easier to use.
     * Not going to code an atlas solution here... are we? */
    SDL_RenderCopy(renderer, tex, NULL, rect);
}

void draw_alpha_line(void){    
    if (!init_complete)
        init_surface();
    draw_alpha_line_proc();
}


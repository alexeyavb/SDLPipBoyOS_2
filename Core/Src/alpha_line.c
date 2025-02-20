#include "common_defs.h"
#include "common.h"
#include "alpha_line.h"

#define DEF_ALPHA_LINE_WIDTH (int)(DEF_SCREEN_WIDTH)
#define DEF_ALPHA_LINE_HEIGHT (int)(DEF_SCREEN_HEIGHT/18)

#define DEF_ALINE_DELAY 500
#define DEF_ALPHA_LINE_SCALE 1.1f

// #25292e
#define DEF_ALPHA_LINE_CLR_R 0x25
#define DEF_ALPHA_LINE_CLR_G 0x29
#define DEF_ALPHA_LINE_CLR_B 0x2e
#define DEF_ALPHA_LINE_ACHANNEL 0xfa
#define DEF_RECT_HEIGHT  (int)(((int) DEF_SCREEN_HEIGHT/3)/12)
extern SDL_Window *window;
extern SDL_Renderer *renderer;

static bool thread_running = true;
static bool init_complete = false;
static int y = (-DEF_RECT_HEIGHT);
static const int interval = 22;
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
    rect = (SDL_Rect*)malloc(sizeof(struct SDL_Rect));

    // #4fafaffa
    surf = SDL_CreateRGBSurface(0, DEF_ALPHA_LINE_WIDTH, DEF_ALPHA_LINE_HEIGHT, 32, 0x00, 0x00, 0xaf, 0xaf);
    SDL_SetSurfaceBlendMode(surf, SDL_BLENDMODE_BLEND);

    tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_SetTextureAlphaMod(tex, 0xa8);
    init_complete = true;
}

void deinit_surface(void){
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
    free(rect);
    rect = NULL;
}

void draw_alpha_line_proc(void){
    if(SDL_GetTicks() >= current_time){
        int delta = rand() % DEF_SCREEN_HEIGHT - DEF_ALPHA_LINE_HEIGHT;
        current_time = SDL_GetTicks() + interval;
        y = (DEF_SCREEN_HEIGHT >= y ) ? delta : (-DEF_RECT_HEIGHT);
    }
    SDL_SetRenderDrawColor(renderer, DEF_ALPHA_LINE_CLR_R, DEF_ALPHA_LINE_CLR_G, DEF_ALPHA_LINE_CLR_B, DEF_ALPHA_LINE_ACHANNEL);
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

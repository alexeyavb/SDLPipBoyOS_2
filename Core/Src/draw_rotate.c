#include "common.h"
#include "common_defs.h"
#include "draw_rotate.h"

extern SDL_Rect mainmenufullrect;
extern int tdraw_rotate(void* data);
bool ldraw_rotate_active;

PGLOBAL_VARS init_draw_rotate(void){  
    PGLOBAL_VARS glob = malloc(sizeof(global_vars));
    glob->surface = IMG_Load("../data/jpg/motorcicle/scout.jpg\0"); 
    SDL_SetSurfaceBlendMode(glob->surface, SDL_BLENDMODE_BLEND);

    glob->destination = malloc(sizeof(SDL_Rect));
    glob->destination->x = (int)(DEF_SCREEN_WIDTH/2) - (int)(DEF_SCREEN_WIDTH/6);
    glob->destination->y = (mainmenufullrect.y * 2) + mainmenufullrect.h;
    glob->destination->h = DEF_SCREEN_HEIGHT - mainmenufullrect.y * 2 - glob->destination->y;
    glob->destination->w = DEF_SCREEN_WIDTH - mainmenufullrect.x - glob->destination->x;
    glob->angle = 360.0f;
    glob->zoom_step = 1.0f;
    glob->no_rotate = 200;
    glob->next_rot_frame_delay = 100;

    return glob;
}

void deinit_draw_rotate(void* glob){    
    free((SDL_Rect *)((PGLOBAL_VARS)glob)->destination);
    SDL_FreeSurface(((PGLOBAL_VARS)glob)->surface);
    free((PGLOBAL_VARS)glob);
}

void ldraw_rotate(void* variables_struct){

    PGLOBAL_VARS pglob = (global_vars*)(variables_struct);
    // int next_rot_frame_delay = pglob->next_rot_frame_delay;
    // int next_rot_frame = pglob->next_rot_frame;
    // double angle = pglob->angle;
    // int no_rotate = pglob->no_rotate;
    // int nul_angle_delay = pglob->nul_angle_delay;
    // double zoom_step = pglob->zoom_step;

    // SDL_Rect* destination = pglob->destination;
    // SDL_Surface* surface = pglob->surface;

    // angle = 360.0f;
    // zoom_step = 1.0f;
    // zoom_step = (360>=angle && 1.0f >= zoom_step ? zoom_step+0.1f : 0.5);        

    if(SDL_GetTicks() >= pglob->next_rot_frame && pglob->nul_angle_delay >= pglob->no_rotate){        
        pglob->angle = ( 360.0f >= pglob->angle ?  pglob->angle + 40 : 0.0f);
        pglob->next_rot_frame = SDL_GetTicks() + pglob->next_rot_frame_delay;
    }

    SDL_Surface* rotated = rotozoomSurface(pglob->surface, pglob->angle, pglob->zoom_step, 0);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, rotated); 

    SDL_SetTextureAlphaMod(texture, 0x5d);        
    SDL_FreeSurface(rotated);
    SDL_RenderCopy(renderer, texture, NULL, pglob->destination);
    SDL_DestroyTexture(texture);

    pglob->nul_angle_delay = 360.0f == pglob->angle && pglob->nul_angle_delay >= pglob->no_rotate ? 0 : pglob->nul_angle_delay + 1;

    // double angle = -(atan2(destination.x + destination.w/2 , destination.y + destination.h/2)) * (180/M_PI) - 90.0f;
}

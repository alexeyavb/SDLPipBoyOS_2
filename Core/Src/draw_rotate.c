#include "common.h"
#include "common_defs.h"
#include "draw_rotate.h"

extern SDL_Rect mainmenufullrect;
extern int tdraw_rotate(void* data);
bool ldraw_rotate_active;

PGLOBAL_VARS init_draw_rotate(void){  
    PGLOBAL_VARS glob = malloc(sizeof(global_vars));
    glob->surface = IMG_Load("../data/jpg/motorcicle/scout.jpg\0"); 
    SDL_SetSurfaceBlendMode(glob->surface, SDL_BLENDMODE_NONE);

    glob->destination = malloc(sizeof(SDL_Rect));
    glob->destination->x = (int)(DEF_SCREEN_WIDTH/2) - (int)(DEF_SCREEN_WIDTH/6);
    glob->destination->y = (mainmenufullrect.y * 2) + mainmenufullrect.h;
    glob->destination->h = DEF_SCREEN_HEIGHT - mainmenufullrect.y * 2 - glob->destination->y;
    glob->destination->w = DEF_SCREEN_WIDTH - mainmenufullrect.x - glob->destination->x;
    glob->angle = 360.0f;
    glob->zoom_step = 1.0f;
    glob->no_rotate = false;
    glob->next_rot_frame_delay = 10;
    glob->next_rotate_time = 0;
    glob->stable_delay = 6000;
    glob->rotation_loop = true;

    return glob;
}

void deinit_draw_rotate(void* glob){    
    free((SDL_Rect *)((PGLOBAL_VARS)glob)->destination);
    SDL_FreeSurface(((PGLOBAL_VARS)glob)->surface);
    free((PGLOBAL_VARS)glob);
}

void ldraw_rotate(void* variables_struct){

    PGLOBAL_VARS pglob = (global_vars*)(variables_struct);

    if(!pglob->no_rotate){
        if(SDL_GetTicks() >= pglob->next_rot_frame){

            if(360.0f >= pglob->angle){
                pglob->angle += 20;
            } else{
                pglob->angle *= 0.0f;
                pglob->next_rotate_time = SDL_GetTicks() + pglob->stable_delay;
                pglob->no_rotate = true;
            } 
            
            pglob->next_rot_frame = SDL_GetTicks() + pglob->next_rot_frame_delay;
        }
    }
    else {
        if(SDL_GetTicks() >= pglob->next_rotate_time){
            pglob->no_rotate = false;
            pglob->angle = 0.00f;            
        }
    }

    SDL_Surface* rotated = rotozoomSurface(pglob->surface, pglob->angle, pglob->zoom_step, 0);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, rotated); 

    SDL_SetTextureAlphaMod(texture, 0x5d);        
    SDL_FreeSurface(rotated);
    SDL_RenderCopy(renderer, texture, NULL, pglob->destination);
    SDL_DestroyTexture(texture);
}

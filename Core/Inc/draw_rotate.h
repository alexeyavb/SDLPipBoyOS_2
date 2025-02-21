#ifndef __DRAW_ROTATE_H__
#define __DRAW_ROTATE_H__
#include <SDL2/SDL.h>
typedef struct {
    int next_rot_frame_delay;
    int no_rotate;
    int next_rot_frame;
    double angle;        
    int nul_angle_delay;
    double zoom_step;
    SDL_Surface* surface;
    SDL_Rect* destination;
} global_vars, *PGLOBAL_VARS, **LPGLOBAL_VARS;

extern PGLOBAL_VARS init_draw_rotate(void);
extern void deinit_draw_rotate(void* glob);
extern void ldraw_rotate(void* variables_struct);

#endif //__DRAW_ROTATE_H__
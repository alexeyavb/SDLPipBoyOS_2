#include "common_defs.h"
#include "common.h"
#include "matrix_rain.h"
#include <time.h>
static int app_running = 1;

int matrix_rain(void){
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            app_running = 0;
        }
    }
    
    return(EXIT_SUCCESS);
}
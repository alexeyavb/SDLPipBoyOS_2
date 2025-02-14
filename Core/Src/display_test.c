#include "common_defs.h"
#include "common.h"
#include "display_test.h"
#include "fps.h"

#define SCREENS_PER_SECOND 1.0
#define MAX_RECT_POS DEF_SCREEN_WIDTH-12

#ifndef EXIT_SUCCESS
    #define EXIT_SUCCESS 0
#endif

int display(void){
    SDL_Event event;
    SDL_Rect rect;
    SDL_Renderer *renderer;
    SDL_Window *window;
    unsigned int initial_time;

    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(DEF_SCREEN_WIDTH, DEF_SCREEN_HEIGHT, 0, &window, &renderer);
    rect.y = 4;
    rect.w = DEF_SCREEN_WIDTH - 8;
    rect.h = DEF_SCREEN_HEIGHT- 8;
    initial_time = SDL_GetTicks();
    common_fps_init();
    while (1) {
        rect.x = ((int)(
                SCREENS_PER_SECOND
                * (MAX_RECT_POS)
                * (SDL_GetTicks() - initial_time) / 1000.0))
                % (MAX_RECT_POS);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 124);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
        common_fps_update_and_print();
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
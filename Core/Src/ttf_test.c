#include "common_defs.h"
#include "common.h"
#include "fps.h"
#include "ttf_utils.h"
//
#include "ttf_test.h"

int ttf_main(int argc, char **argv) {
    SDL_Color color;
    SDL_Event event;
    SDL_Rect rect;
    SDL_Renderer *renderer;
    SDL_Window *window;
    char *font_path, text[MAX_STRING_LEN];

    /* CLI arguments. */
    if (argc == 1) {
        // font_path = "../FreeSans.ttf";
        font_path = "../VT323-Regular.ttf";
    } else if (argc == 2) {
        font_path = argv[1];
    } else {
        fprintf(stderr, "error: too many arguments\n");
        exit(EXIT_FAILURE);
    }

    /* initialize variables. */
    color.r = 0;
    color.g = COMMON_COLOR_MAX;
    color.b = 0;
    color.a = 220;

    /* Init window. */
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(
        DEF_SCREEN_WIDTH,
        DEF_SCREEN_HEIGHT,
        0,
        &window,
        &renderer
    );

    /* Init TTF. */
    TTF_Init();
    TTF_Font *font = TTF_OpenFont(font_path, 18);
    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }

    /* Main loop. */
    common_fps_init();
    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
            break;
        }

        /* Use TTF. */
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 16);
        SDL_RenderClear(renderer);

        render_text(renderer, 0, 0,               "hello", font, &rect, &color);
        render_text(renderer, 0, rect.y + rect.h, "world", font, &rect, &color);
        snprintf(text, MAX_STRING_LEN, "%u", (unsigned int)(time(NULL) % 1000));
        render_text(renderer, 0, rect.y + rect.h, text, font, &rect, &color);
        SDL_RenderPresent(renderer);

        common_fps_update_and_print();
    }

    /* Cleanup. */
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
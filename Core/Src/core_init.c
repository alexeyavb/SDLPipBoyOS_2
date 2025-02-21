#include "common_defs.h"
#include "common.h"
#include "fps.h"
#include "ttf_utils.h"
#include "core_init.h"

TTF_Font* TTF_FontCache[DEF_FONT_CACHE_SZ];
SDL_Renderer *renderer;
SDL_Window *window;
SDL_RendererInfo renderinfo;

extern int fonts_load(void);

int core_init(){
    
    int errlvl = 0;
    SDL_SetHint(SDL_HINT_VIDEO_DOUBLE_BUFFER, "kmsdrm");
    errlvl = SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    if(errlvl != 0){
        perror("Error initialise SDL core");
    }    
    errlvl = TTF_Init();
    if(errlvl != 0){
        perror("Error initialise TTF subcore");
    }    
    // SDL_CreateWindowAndRenderer(
    //     DEF_SCREEN_WIDTH,
    //     DEF_SCREEN_HEIGHT,
    //     0,
    //     &window,
    //     &renderer
    // );

    // GL init
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);    

    window = SDL_CreateWindow("Matrix-Code",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            DEF_SCREEN_WIDTH,
            DEF_SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
        );

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengles");
    SDL_SetHint(SDL_HINT_RENDER_LOGICAL_SIZE_MODE, "overscan");
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    if(NULL == glcontext){
        SDL_Log("error creating glcontext");        
    }
    
    SDL_SetWindowKeyboardGrab(window, SDL_TRUE);
    
    renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetHint(SDL_HINT_RENDER_LINE_METHOD, "3");

    SDL_RenderSetLogicalSize(renderer, DEF_SCREEN_WIDTH, DEF_SCREEN_HEIGHT);
    // Disable any scaling (logical size scaling)
    SDL_RenderSetScale(renderer, 1.0f, 1.0f);  // No scaling

    fonts_load();

    SDL_GetRendererInfo(renderer, &renderinfo);
    SDL_GetRenderDriverInfo(0, &renderinfo);

    common_fps_init();
    return 0;
}

int core_deinit(void){
    /* Cleanup. */
    int errlvl = 0;
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return errlvl;
}

int fonts_load(void){
    int errlvl = 0;
    TTF_FontCache[DEF_FONT_IDX] = TTF_OpenFont(DEF_FONT_NAME, DEF_FONT_SIZE);
    TTF_FontCache[DEF_FONT_20_IDX] = TTF_OpenFont(DEF_FONT_NAME, DEF_FONT_SIZE_20);
    TTF_FontCache[DEF_FONT_22_IDX] = TTF_OpenFont(DEF_FONT_NAME, DEF_FONT_SIZE_22);
    TTF_FontCache[DEF_FONT_24_IDX] = TTF_OpenFont(DEF_FONT_NAME, DEF_FONT_SIZE_24);
    TTF_FontCache[DEF_FONT_26_IDX] = TTF_OpenFont(DEF_FONT_NAME, DEF_FONT_SIZE_26);
    return errlvl;
}
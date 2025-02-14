#include "common.h"
#include "common_defs.h"
#include "mainmenu.h"
#include "ttf_utils.h"

SDL_Rect mainmenufullrect;
extern SDL_Rect  screen_rect;
extern SDL_Color color;

typedef struct mainmenuitem{
    int itm_id;
    int isactive;
    const char *itm_label;
    int submenu_count;
    SDL_Rect* item_rect;
    struct mainmenuitem** subitems;
} **LPMAIN_MENU_ITEM, *PMAIN_MENU_ITEM, MAIN_MENU_ITEM;

typedef struct label_struct{
    const char* label;
    const struct label_struct* subitems;
    void *action_cb;
} **LPLABEL_STRUCT, *PLABEL_STRUCT, LABEL_STRUCT;

static PMAIN_MENU_ITEM mainmenu[MM_SIZE];

static const LABEL_STRUCT mm_star_items[MM_STAR_SIZE]= {
    {"About",       NULL, NULL}, 
    {"System set",  NULL, NULL}, 
    {"Services",    NULL, NULL}
};

static const LABEL_STRUCT mainmenulabels[MM_SIZE]= {
    {"[ * ]",       mm_star_items, NULL}, 
    {"[ Main ]",    NULL, NULL},  
    {"[ File ]",    NULL, NULL}, 
    {"[ Edit ]",    NULL, NULL},
    {"[ Select ]",  NULL, NULL}, 
    {"[ View ]",    NULL, NULL}, 
    {"[ Window ]",  NULL, NULL}, 
    {"[ Help ]",    NULL, NULL}
};


int active_menu_item = 0;

void main_menu_init(void*  menu_item){

    for(int i = 0; i < MM_SIZE; i++){
        mainmenu[i] = malloc(sizeof(struct mainmenuitem));
        mainmenu[i]->isactive = false;
        mainmenu[i]->item_rect = malloc(sizeof(SDL_Rect));
        mainmenu[i]->itm_id = i;
        mainmenu[i]->itm_label = mainmenulabels[i].label;
        mainmenu[i]->submenu_count = 0;
        mainmenu[i]->subitems = NULL;
    }
    return;
}

void main_menu_deinit(void){
    for(int i = 0; i < MM_SIZE; i++){
        free(mainmenu[i]->item_rect);
        free(mainmenu[i]);
    }
}

// Yellow theme
/*
static const SDL_Color mmcolor = {
    0x3b, 0x23, 0x00, 0xff
};

static SDL_Color mmpcolor = {
    0xff, 0xf8, 0xc5, 0xff
};

static SDL_Color mmacolor = {
    0xbf, 0x87, 0x00, 0xf0
};
*/

// green theme
static const SDL_Color mmcolor = {
    // #2f4a21ff
    0x2f, 0x4a, 0x21, 0xff
};

static SDL_Color mmpcolor = {
    // 0xff, 0xf8, 0xc5, 0xff
    // 0xe3, 0xf2, 0xb5, 0xfa
    // #9bd039ff
    0x9b,0xd0,0x39, 0xff    
};

static SDL_Color mmacolor = {
    // 0xbf, 0x87, 0x00, 0xf0
    //     #6c9d2ffb
    0x6c, 0x9d, 0x2f, 0xfb
};

void draw_main_menu_header(void){
    
    mainmenufullrect.x = screen_rect.x + 2;
    mainmenufullrect.y = screen_rect.y + 2;
    mainmenufullrect.w = screen_rect.w - 4;
    mainmenufullrect.h = screen_rect.x + 22;
    
    SDL_SetRenderDrawColor(renderer, mmcolor.r, mmcolor.g, mmcolor.b, mmcolor.a);
    SDL_RenderFillRect(renderer, &mainmenufullrect);
}

void draw_main_menu_items(void){
    TTF_Font *mmfont = TTF_FontCache[DEF_FONT_24_IDX];
    SDL_Rect current_rect;

    for(int i=0; i < MM_SIZE; i++){
        render_text(renderer, 
            mainmenufullrect.x + 1 + (0==i ? 0 : current_rect.x + current_rect.w + 1),
                mainmenufullrect.y + 1, 
            mainmenu[i]->itm_label, 
            mmfont, 
            &current_rect, 
            active_menu_item == i ? &mmpcolor : &mmacolor);
            
            if(active_menu_item == i) {
                SDL_RenderFillRect(renderer, &current_rect);
                render_text(renderer, 
                    current_rect.x,
                    mainmenufullrect.y + 1, 
                    mainmenu[i]->itm_label,
                    mmfont, 
                    &current_rect,
                    active_menu_item == i ? &mmpcolor : &mmacolor);
            }        
            memcpy((SDL_Rect*)(mainmenu[i]->item_rect), &current_rect, sizeof(SDL_Rect));
    }
}

void draw_submenu_items(PMAIN_MENU_ITEM menuitem){

}
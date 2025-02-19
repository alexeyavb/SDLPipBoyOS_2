#include "common.h"
#include "common_defs.h"
#include "mainmenu.h"
#include "ttf_utils.h"

SDL_Rect mainmenufullrect;
extern SDL_Rect  screen_rect;
extern SDL_Color color;
// Navigation counters
int active_menu_item;

// action flags
bool expandorcall_cb; 
bool expandordown_cb;

typedef struct mainmenuitem{
    int itm_id;
    bool isactive;
    bool isexpanded;
    bool isexpandable;
    int subitemscount;
    const char *itm_label;    
    SDL_Rect* item_rect;
    struct mainmenuitem* subitems;
    void* action_cb;
} **LPMAIN_MENU_ITEM, *PMAIN_MENU_ITEM, MAIN_MENU_ITEM;

typedef struct label_struct{    
    const char* label;
    bool Expandable;
    int subitemscount;
    const struct label_struct* subitems;
    void *action_cb;
} **LPLABEL_STRUCT, *PLABEL_STRUCT, LABEL_STRUCT;

// current active menu path strucuture
typedef struct activemmbtree{
    struct activemmbtree* parent;
    struct activemmbtree* next;
    int idx;
    PMAIN_MENU_ITEM menuitem;
} **LPACTIVE_MM_BTREE, *PACTIVE_MM_BTREE, ACTIVE_MM_BTREE;

static PMAIN_MENU_ITEM mainmenu[MM_SIZE];

static const LABEL_STRUCT mm_star_items[MM_STAR_SIZE]= {
    {"About",       false, 0, NULL, NULL}, 
    {"System set",  false, 0, NULL, NULL}, 
    {"Services",    false, 0, NULL, NULL}
};

static const LABEL_STRUCT mm_main_items[MM_MAIN_SIZE]= {
    {"Get updates", false, 0, NULL, NULL}, 
    {"Settings",    false, 0, NULL, NULL}, 
    {"Hide",        false, 0, NULL, NULL},
    {"Hide others", false, 0, NULL, NULL}
};

static const LABEL_STRUCT mm_file_items[MM_FILE_SIZE]= {
    {"New",    false, 0, NULL, NULL}, 
    {"Open",         false, 0, NULL, NULL}, 
    {"Oppen as",          false, 0, NULL, NULL},
    {"Close",   false, 0, NULL, NULL},
    {"Quit",             false, 0, NULL, NULL}
};

static const LABEL_STRUCT mainmenulabels[MM_SIZE]= {
    {"[ * ]",       true, MM_STAR_SIZE,     mm_star_items, NULL}, 
    {"[ Main ]",    true, MM_MAIN_SIZE,    mm_main_items, NULL},
    {"[ File ]",    true, MM_FILE_SIZE,    mm_file_items, NULL},
    {"[ Edit ]",    false, 0, NULL, NULL},
    {"[ Select ]",  false, 0, NULL, NULL},
    {"[ View ]",    false, 0, NULL, NULL},
    {"[ Window ]",  false, 0, NULL, NULL},
    {"[ Help ]",    false, 0, NULL, NULL}
};

void init_menu_item(PLABEL_STRUCT current_label_struct, PMAIN_MENU_ITEM current_menuitem, int itm_id){
    
    current_menuitem->isactive = false;
    current_menuitem->itm_id = itm_id;    
    current_menuitem->item_rect = malloc(sizeof(struct SDL_Rect));    
    current_menuitem->itm_label = current_label_struct->label;    
    current_menuitem->subitemscount = current_label_struct->subitemscount;

    if(NULL != current_label_struct->action_cb ){
        current_menuitem->action_cb = current_label_struct->action_cb;
        current_menuitem->isexpandable = false;
        current_menuitem->subitemscount = 0;
    }
    
    if(current_label_struct->Expandable){
        if(NULL != current_label_struct->subitems){
            if(current_label_struct->subitemscount > 0){
                current_menuitem->action_cb = NULL;
                current_menuitem->isexpandable = true;
                current_menuitem->subitemscount = current_label_struct->subitemscount;                 
                PMAIN_MENU_ITEM lpsubitem = (struct mainmenuitem*)(malloc(sizeof(struct mainmenuitem)));
                current_menuitem->subitems = lpsubitem;
                for(int i = 0; i < current_label_struct->subitemscount; i++){
                    if(NULL == current_menuitem->subitems)
                        current_menuitem->subitems = (struct mainmenuitem*)(malloc(sizeof(struct mainmenuitem)));
                    init_menu_item(&current_label_struct->subitems[i], current_menuitem->subitems, i) ;
                    current_menuitem = current_menuitem->subitems;
                }
            }
        }
    }
    return;
}

void main_menu_init(void*  menu_item){
    active_menu_item = 0;
    expandorcall_cb = false; 
        
    for(int i = 0; i < MM_SIZE; i++){
        mainmenu[i] = malloc(sizeof(struct mainmenuitem));
        init_menu_item(&mainmenulabels[i], mainmenu[i], i);
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
        mainmenu[i]->isactive = false;

        if(active_menu_item == i) {
            mainmenu[i]->isactive = true;
            SDL_RenderFillRect(renderer, &current_rect);
            render_text(renderer, 
                current_rect.x,
                mainmenufullrect.y + 1, 
                mainmenu[i]->itm_label,
                mmfont, 
                &current_rect,
                active_menu_item == i ? &mmpcolor : &mmacolor);
                
            // draw 1 lvl subitems
            if(mainmenu[i]->isexpandable && mainmenu[i]->isexpanded && (0<mainmenu[i]->subitemscount) && (NULL != mainmenu[i]->subitems)){
                PMAIN_MENU_ITEM currentitem = (PMAIN_MENU_ITEM)(mainmenu[i]);
                SDL_Rect current_subrect = {0};
                for(int si = 0; si<mainmenu[i]->subitemscount; si++){                    
                    render_text(renderer, 
                        mainmenufullrect.x + 1 + (0==si ? current_rect.x + 4 : current_subrect.x + current_subrect.w + 1),
                        mainmenufullrect.y + 1 + current_rect.h+2,                                                
                        currentitem->subitems->itm_label,
                        TTF_FontCache[DEF_FONT_22_IDX],
                        &current_subrect,
                        &mmacolor);

                        if(NULL != currentitem->subitems)
                            currentitem = (PMAIN_MENU_ITEM)currentitem->subitems;
                }
            }
        }        
        memcpy((SDL_Rect*)(mainmenu[i]->item_rect), &current_rect, sizeof(SDL_Rect));
    }
}

 // expand or call cb
 void expandorcallacction(void){
    if (expandorcall_cb){
        if(0 < mainmenu[active_menu_item]->subitemscount){            
            if(NULL != mainmenu[active_menu_item]->subitems)
                printf("Draw main menu %s, %d subitems\n", mainmenu[active_menu_item]->itm_label, mainmenu[active_menu_item]->subitemscount);
                mainmenu[active_menu_item]->isexpanded = !mainmenu[active_menu_item]->isexpanded;
        }
        else{
            if(NULL != mainmenu[active_menu_item]->action_cb){
                printf("execute menu callback %s\n", mainmenu[active_menu_item]->itm_label);
            }
            else
                printf("No action or submenu found for %s\n", mainmenu[active_menu_item]->itm_label);
        }        
    }
    expandorcall_cb = false;
}

// expand or call cb
void expandordownacction(void){
    if (expandordown_cb){
        if(0 < mainmenu[active_menu_item]->subitemscount){
        }
    }
    expandordown_cb = false;
}

void draw_submenu_items(PMAIN_MENU_ITEM menuitem){

}
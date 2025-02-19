#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#define MM_SIZE 8
#define MM_STAR_SIZE 3
#define MM_MAIN_SIZE 4
#define MM_FILE_SIZE 5

#define MM_MENU_ASZE    3
#define MM_MENU_LBL     0
#define MM_MENU_ARRAY   1
#define MM_MENU_ACTN    2

struct mainmenuitem;
struct label_struct;

extern SDL_Rect mainmenufullrect;

extern void main_menu_init(void* menu_item);
extern void main_menu_deinit(void);

#endif
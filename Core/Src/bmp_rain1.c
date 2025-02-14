#include "common_defs.h"
#include "common.h"
#include "bmp_font.h"

#define STRIPS 250				//the number of streams of text scrolling down the screen
#define SCALE 1					//the scale at which each char in the stream is rendered
#define V_GAP .8				//the vertical gap between chars in the stream

int mtx_init_one(int width, int height);
void mtx_vert_txt(char *str, int x, int y);
void mtx_check_bounds(void);

struct strip {
	int x;
	float y;
	float speed;
	int len;
	// char str[32];
    char str[48];
};

//function prototypes
extern SDL_Window* window;	//The window we'll be rendering to
extern SDL_Renderer *renderer;		//The renderer SDL will use to draw to the screen
extern int common_fps_update_and_print(void);

static SDL_Texture *font_t;			//The texture that holds the font
struct strip strips[STRIPS];		//the array that holds the strips of text and its properties

int bmp_rain1(void) {
	//SDL Window setup
	if (mtx_init_one(DEF_SCREEN_WIDTH, DEF_SCREEN_HEIGHT) == 1) {		
		return 0;
	}	
	int sleep = 0;
	int quit = 0;
	Uint32 next_game_tick = SDL_GetTicks();
	
	//set the scale of the font
	font_set_scale(SCALE);
	font_set_v_gap(V_GAP);
	
	//populate the strip array
	for (int i = 0; i < STRIPS; i++) {
		
		int char_width = get_char_width();
		strips[i].x = (rand() % DEF_SCREEN_WIDTH / char_width) * char_width;
		strips[i].y = (strips[i].len * get_char_height()) * -1;
		strips[i].speed = (float) (rand() + RAND_MAX * .5) / RAND_MAX * 2.5;
		strips[i].len = rand() % 19 + 5;
		
		//populate str property
		for (int j = 0; j < strips[i].len; j++) {			
			int r = rand() % 1000;
			//random lower case
			if (r < 333) {
				strips[i].str[j] = rand() % 25 + 97;
			//random uppercase
			} else if (r > 333 && r < 666) {
				strips[i].str[j] = rand() % 25 + 65;
			} else {
				strips[i].str[j] = rand() % 9 + 48;
			}
		}
	}

    // matrix timeout beg 10.5sec
    Uint32 start_tick = SDL_GetTicks();
    static const int matrix_render_time = 10656;
    Uint32 lasttick = matrix_render_time + start_tick;
    // end t/o
  
	//render loop
	while(quit == 0) {
		//check for new events every frame
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                quit = 1;
            }
        }

		mtx_check_bounds();

		SDL_RenderClear(renderer);
						
		//draw the strips
		for (int i = 0; i < STRIPS; i++) {
			//draw the strips
			mtx_vert_txt(strips[i].str, strips[i].x, (int)strips[i].y);
			strips[i].y += strips[i].speed;
		}
		
		//draw to the screen
		SDL_RenderPresent(renderer);
				
		//time it takes to render 1 frame in milliseconds
		next_game_tick += 1000 / 60;
		sleep = next_game_tick - SDL_GetTicks();
	
		if( sleep >= 0 ) {            				
			SDL_Delay(sleep);
		}

        common_fps_update_and_print();
        if(SDL_GetTicks() >= lasttick)
            quit = 1;        
	}
    SDL_DestroyTexture(font_t);
	return 0;
}

void mtx_vert_txt(char *str, int x, int y) {	
	int i;
	int char_height = get_char_height();
	int length = strlen(str);
	
	SDL_SetTextureColorMod(font_t, 20, 160, 20);
	
	for (i = 0; i < length; i++) {
		
		float tail = (float) i / length;
		
		//draw the first few chars with a darker colour
		if (tail <= .4) {
			
			float grad = (float) i / (length * .4);
			
			if (i == 0 ) {
					
				grad = .15;
			}
			
			SDL_SetTextureAlphaMod(font_t, 255 * grad);
		}
		
		//draw the last element a different colour
		if (i == strlen(str) - 1) {

			SDL_SetTextureColorMod(font_t, 100, 255, 100);
		}

		print_char(str[i], x, y, renderer, font_t);
		y += char_height;		
		
		if (rand() % 1000 < 10) {			
			int r = rand() % 1000;				
			//random lower case (ascii values)
			if (r < 333) {				
				str[i] = rand() % 25 + 97;			
			//random uppercase
			} else if (r > 333 && r < 666) {				
				str[i] = rand() % 25 + 65;			
			//random number
			} else {
				str[i] = rand() % 9 + 48;
			}
		}
	}
}

void mtx_check_bounds(void) {	
	int i;
	for (i = 0; i < STRIPS; i++) {		
		if (strips[i].y >= DEF_SCREEN_HEIGHT) {			
			int char_width = get_char_width() * SCALE;
			strips[i].y = (strips[i].len * get_char_height()) * -1;
			strips[i].x = (rand() % DEF_SCREEN_WIDTH / char_width) * char_width;
		}
	}
}

int mtx_init_one(int width, int height) {
	//Create window	
    font_t = load_font("mtx.bmp",renderer);	
	if (window == NULL) { 
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}
	if (font_t == NULL) { 
		printf("Font texture could not be created!\n");
		return 1;
	}
	return 0;
}
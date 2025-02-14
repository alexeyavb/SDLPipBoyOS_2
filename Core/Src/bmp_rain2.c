#include "common_defs.h"
#include "common.h"
#include "bmp_font.h"

#define STREAM_FACTOR 1.8f	//number of chars streaming down
#define SCALE .81f			//scale at which the chars are drawn
#define H_GAP .74f		//gap between the chars horizontally
#define V_GAP .77f		//gap between the chars vertically

//function prototypes
int mtx_init(int w, int h);
void fade(int col, int row, unsigned char a[col][row]);

//structure to define the stream of chars cascading down the window
struct stream {	
	float x;
	float y;
	float speed;
};

//globals
extern SDL_Window* window;	//The window we'll be rendering to
extern SDL_Renderer *renderer;		//The renderer SDL will use to draw to the screen
extern int common_fps_update_and_print(void);

SDL_Texture *font_t;		//The texture that holds the font

int bmp_rain2(void) {
		
	//SDL Window setup
	if (mtx_init(DEF_SCREEN_WIDTH, DEF_SCREEN_HEIGHT) == 1) {
		
		return 0;
	}
	
	//font settings
	font_set_scale(SCALE);
	font_set_v_gap(V_GAP);
	font_set_h_gap(H_GAP);
	
	int width, height;
	
	SDL_GetWindowSize(window, &width, &height);
	
	printf("window dimentions w = %d, h = %d\n", width, height);
	
	//calculate the dimensions of the  two arrays that will hold the chars and their alpha values respectively 
	int cols = width / get_char_width();
	int rows = height / get_char_height();
	
	printf("matrix dimentions w = %d, h = %d\n", cols, rows);
	
	char char_grid[cols][rows];
	unsigned char alpha_grid[cols][rows];
	
	int x,y;
	
	//populate grid of random chars and set all alpha values to 0
	for (x = 0; x < cols; x++) {

		for (y = 0; y < rows; y++) {
		 
			int r = rand() % 1000;
			
			//char_grid values random lower case
			if (r < 333) {
				
				char_grid[x][y] = rand() % 25 + 97;
			
			//random uppercase
			} else if (r > 333 && r < 666) {
				
				char_grid[x][y] = rand() % 25 + 65;
			
			} else {
			
				char_grid[x][y] = rand() % 9 + 48;
			}
			
			//alpha values
			alpha_grid[x][y] = 0;
		}
	}
	
	//array to hold all the streams
	int num_streams = cols * STREAM_FACTOR;
	struct stream streams[num_streams];
	
	printf("streams %d\n", num_streams);
	
	//populate streams
	for (x = 0; x < num_streams; x++) {
	
		streams[x].x = rand() % cols;
		streams[x].y = 0;
		streams[x].speed = (float) (rand() % 150) / 1000 + .05;
	}
	
	int sleep = 0;
	int quit = 0;
	Uint32 next_game_tick = SDL_GetTicks();
	
    // matrix timeout beg 4sec
    Uint32 start_tick = SDL_GetTicks();
    static const int matrix_render_time = 10656;
    Uint32 lasttick = matrix_render_time + start_tick;
    // end t/o

	//render loop
	while(quit == 0) {
	
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                quit = 1;
            }
        }

		//draw background
		SDL_RenderClear(renderer);
		
		int x,y;
		int px = 0;
		int py = 0;
		
		//print all chars to  the screen
		for (x = 0; x < cols; x++) {
			
			for (y = 0; y < rows; y++) {
				
				unsigned char val = alpha_grid[x][y];
				
				if (val > 240) {
					
					SDL_SetTextureColorMod(font_t, 100, 255, 100);
                    // SDL_SetTextureColorMod(font_t, 255, 100, 100);
					
				} else {
					
					SDL_SetTextureColorMod(font_t, 20, 160, 20);
                    // SDL_SetTextureColorMod(font_t, 220, 40, 40);
				}
				
				//improve performance not rendering the char texture if the cell has a low alpha value
				if (val > 45) {				
					SDL_SetTextureAlphaMod(font_t, val);
					print_char(char_grid[x][y], px, py, renderer, font_t);
				}
				
				py += get_char_height();
			}
			
			py = 0;
			px += get_char_width();
		}
		
		// int i;
		
		//render the streams
		for (int i = 0; i < num_streams; i++) {
			
			if (streams[i].y > rows) {
				
				streams[i].y = 0;
				streams[i].x = rand() % cols;
			}
			
			alpha_grid[(int)streams[i].x][(int)streams[i].y] = 255;
			streams[i].y += streams[i].speed;
		}
		
		fade(cols, rows, alpha_grid);
		
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


//fade out the the alpha_grid values by 1;
void fade(int col, int row, unsigned char a[col][row]) {
	
	int x,y;
	
	for (x =0; x < col; x++) {
		
		for (y =0; y < row; y++) {
				
			if (a[x][y] > 0) {
				
				a[x][y] -= 1;
			}
		}
	}
}

int mtx_init(int width, int height) {
	//set up font texture
	font_t = load_font("../mtx.bmp",renderer);
	
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

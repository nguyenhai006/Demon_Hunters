#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_	


#include<cmath>
#include<vector>
#include<string>
#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

static SDL_Window* g_window = NULL;	
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

// Screen	
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;  

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 255;

// animation charactor
const int animIndex = 4;

#define TILE_SIZE 64 // kích thước của 1 ô
#define BLANK_TILE 0

#define MAX_MAP_X 57		// số ô theo chiều ngang
#define MAX_MAP_Y 57		// số ô theo chiều dọc

#define MAX_X 3648			// số pixel của map
#define MAX_Y 3648


typedef struct  {
	int left_;
	int right_;
	int up_;
	int down_;
}Input;

typedef struct  {
	int start_x_;	// vị trí bắt đầu theo chiều ngang
	int start_y_;	// vị trí bắt đầu theo chiều dọc
	int max_x_;		// số ô theo chiều ngang
	int max_y_;		// số ô theo chiều dọc
	int tile[MAX_MAP_Y][MAX_MAP_X];	// mảng lưu trữ thông tin các ô
	const char* file_name_;	// tên file chứa thông tin map
} Map;

#endif // !COMMON_FUNCTION_H_
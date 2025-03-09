#include"stdafx.h"
#include"CommonFunc.h"
#include"BaseObject.h"

BaseObject g_background;

bool InitData();

bool LoadBackground();

void Close();

int main(int argc, char* argv[]) {
	if (InitData() == false)
	{
		return -1;
	}
	if (LoadBackground() == false)
	{
		return -1;
	}
	bool is_quit = false;
	while (!is_quit)
	{
		while (SDL_PollEvent(&g_event) != 0)
		{
			if (g_event.type == SDL_QUIT)
			{
				is_quit = true;
			}
		}
		SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR); // set màu cho renderer
		SDL_RenderClear(g_screen); // clear renderer
		g_background.Render(g_screen, NULL); // render ảnh nền
		SDL_RenderPresent(g_screen); // update renderer
	}
	Close();
	return 0;
}

bool InitData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO); // khởi tạo SDL
	if (ret < 0) // nếu không thể khởi tạo SDL
	{
		return false;
	}
	else {
		return false;
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); // set chất lượng render
	g_window = SDL_CreateWindow("Demon Hunter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); // tạo cửa sổ
	if (g_window == NULL) // nếu cửa sổ bằng NULL
	{
		success = false;
	}
	else
	{
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED); // tạo renderer
		if (g_screen == NULL) // nếu renderer bằng NULL
		{
			success = false;
		}
		else
		{
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR); // set màu cho renderer
			int imgFlags = IMG_INIT_PNG; // set flag cho ảnh
			if (!(IMG_Init(imgFlags) && imgFlags)) // nếu không thể khởi tạo ảnh
			{
				success = false;
			}
		}
	}
	return success;
}

bool LoadBackground()
{
	bool ret = g_background.LoadImg("img//background.png", g_screen); // load ảnh nền
	if (ret == false) // nếu không thể load ảnh
	{
		return false;
	}
	return true;
}

void Close()
{
	g_background.Free(); // giải phóng ảnh nền
	SDL_DestroyRenderer(g_screen); // giải phóng renderer
	g_screen = NULL; // gán NULL cho renderer
	SDL_DestroyWindow(g_window); // giải phóng cửa sổ
	g_window = NULL; // gán NULL cho cửa sổ
	IMG_Quit(); // quit ảnh
	SDL_Quit(); // quit SDL
}
#include"stdafx.h"
#include"CommonFunc.h"
#include"BaseObject.h"
#include"Game_map.h"
#include"player.h"

bool InitData();

void Close();

int main(int argc, char* argv[]) {
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;

	if (InitData() == false)
	{
		std::cerr << "InitData failed!" << std::endl;
		return -1;
	}

	GameMap game_map;
	game_map.LoadMap("map/map01.dat");		//	load map

	game_map.LoadTiles(g_screen);


	player p_player;
	p_player.LoadImg("img//walk_right.png", g_screen);
	p_player.set_clips();

	bool is_quit = false;
	while (!is_quit)
	{
		while (SDL_PollEvent(&g_event) != 0)
		{
			if (g_event.type == SDL_QUIT)
			{
				is_quit = true;
			}
			p_player.HandleInputAction(g_event, g_screen);
		}
		frameStart = SDL_GetTicks();

		SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR); // set màu cho renderer
		SDL_RenderClear(g_screen); // clear renderer

		game_map.DrawMap(g_screen, p_player.GetCamera());

		
		p_player.HandleBullet(g_screen);
		p_player.DoPlayer();

		p_player.Show(g_screen);
		

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}

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
		std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); // set chất lượng render
	g_window = SDL_CreateWindow("Demon Hunter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); // tạo cửa sổ
	if (g_window == NULL) // nếu cửa sổ bằng NULL
	{
		std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else
	{
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED); // tạo renderer
		if (g_screen == NULL) // nếu renderer bằng NULL
		{
			std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR); // set màu cho renderer
			int imgFlags = IMG_INIT_PNG; // set flag cho ảnh
			if (!(IMG_Init(imgFlags) && imgFlags)) // nếu không thể khởi tạo ảnh
			{
				std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
				success = false;
			}
		}
	}
	return success;
}



void Close()
{
	SDL_DestroyRenderer(g_screen); // giải phóng renderer
	g_screen = NULL; // gán NULL cho renderer
	SDL_DestroyWindow(g_window); // giải phóng cửa sổ
	g_window = NULL; // gán NULL cho cửa sổ
	IMG_Quit(); // quit ảnh
	SDL_Quit(); // quit SDL
}

#include"stdafx.h"
#include"CommonFunc.h"
#include"BaseObject.h"
#include"Game_map.h"
#include"player.h"
#include"Enemy.h"
#include"SlimeAndBat.h"

bool InitData();

void Close();

std::vector<Enemy*> enemy_list;
Slime* slime = new Slime();
Bat* bat = new Bat();

int main(int argc, char* argv[]) {
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;

	Uint32 lastSlimeSpawnTime = 0;
	Uint32 lastBatSpawnTime = 0;
	const Uint32 spawnInterval = 5000;								//thời gian spawn quái là 5s

	Uint32 frameTime;
	Uint32 lastTime = SDL_GetTicks();

	srand(static_cast<unsigned int>(time(0)));						//mỗi lần chạy là ra 1 giá trị rand khác nhau

	int spawn_count = 5;
	int playerdamage = 10;

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
		Uint32 current_time = SDL_GetTicks();							//biến để lấy tg hiện tại

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
		float player_x, player_y;
		p_player.GetPosition(player_x, player_y);					//lấy vị trí của người chơi

		if (current_time - lastSlimeSpawnTime >= spawnInterval) {
			slime->UpdateSpawnEnemies(enemy_list, g_screen, lastSlimeSpawnTime, spawnInterval, lastTime, spawn_count, 0);
			lastSlimeSpawnTime = current_time;
		}

		if (current_time > lastTime + 180000) {  // 180000 ms = 3 phút
			if (current_time - lastBatSpawnTime >= spawnInterval * 2) {
				bat->UpdateSpawnEnemies(enemy_list, g_screen, lastBatSpawnTime, spawnInterval * 2, lastTime, spawn_count, 1);
				lastBatSpawnTime = current_time;
			}
		}

		for (int i = 0; i < enemy_list.size(); i++)
		{
			Enemy* p_enemy = enemy_list.at(i);
			if (p_enemy != NULL)
			{
				int enemy_type = p_enemy->GetType();
				p_enemy->set_clipsEnemy();
				p_enemy->ShowEnemy(g_screen, p_player.GetCamera());
				p_enemy->SetTarget(player_x, player_y);
				p_enemy->Move();

			}
		}
		std::vector<Bullet*> bullet_arr = p_player.get_bullet_list();
		for (int bl = 0; bl < bullet_arr.size(); bl++)
		{
			Bullet* p_bullet = bullet_arr.at(bl);
			if (p_bullet != NULL)
			{
				for (int i = 0; i < enemy_list.size(); i++)
				{
					Enemy* obj_enemy = enemy_list.at(i);
					if (obj_enemy != NULL)
					{
						SDL_Rect eRect;
						eRect.x = obj_enemy->GetRect().x;
						eRect.y = obj_enemy->GetRect().y;
						eRect.w = obj_enemy->get_width_frame();
						eRect.h = obj_enemy->get_height_frame();

						SDL_Rect bRect = p_bullet->GetRect();

						bool bCol = SDLCommonFunc::CheckCollision(bRect, eRect);

						if (bCol)
						{
							p_player.RemoveBullet(bl);
							obj_enemy->Free();
							enemy_list.erase(enemy_list.begin() + i);
						}
					}
				}
			}
		}
		
		SDL_RenderPresent(g_screen); // update renderer

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}

	}
	Close();
	return 0;
}

bool InitData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);						// khởi tạo SDL
	if (ret < 0)											// nếu không thể khởi tạo SDL
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
	for (int i = 0; i < enemy_list.size(); i++) {
		delete enemy_list[i];  // Giải phóng từng enemy
	}
	enemy_list.clear();  // Xóa toàn bộ danh sách

	// Giải phóng slime và bat
	delete slime;
	delete bat;

	SDL_DestroyRenderer(g_screen); // giải phóng renderer
	g_screen = NULL; // gán NULL cho renderer
	SDL_DestroyWindow(g_window); // giải phóng cửa sổ
	g_window = NULL; // gán NULL cho cửa sổ
	IMG_Quit(); // quit ảnh
	SDL_Quit(); // quit SDL
}

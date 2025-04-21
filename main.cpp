
#include"stdafx.h"
#include"CommonFunc.h"
#include"BaseObject.h"
#include"Game_map.h"
#include"player.h"
#include"Enemy.h"
#include"SlimeAndBat.h"

bool InitData();

void Close();

void RenderUI(SDL_Renderer* renderer, Uint32 start_time, int kill_count);

TTF_Font* font = NULL;
std::vector<Enemy*> enemy_list;
Slime* slime = new Slime();
Bat* bat = new Bat();

int main(int argc, char* argv[]) {
	
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	int kill_enemy = 0;

	Uint32 frameStart;

	Uint32 lastSlimeSpawnTime = 0;									//lần cuối spawn slime
	Uint32 lastBatSpawnTime = 0;									//lần cuối spawn bat
	const Uint32 spawnInterval = 5000;								//thời gian spawn quái là 5s

	Uint32 frameTime;
	Uint32 lastTime = SDL_GetTicks();
	Uint32 start_time = SDL_GetTicks();								//thời điểm bắt đầu game

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
	int map_width = MAX_MAP_X * TILE_SIZE;
	int map_height = MAX_MAP_Y * TILE_SIZE;

	float x = (map_width - p_player.PlayerWidth()) / 2;
	float y = (map_height - p_player.PlayerHeight()) / 2;
	p_player.SetMapXY(x, y);

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
		p_player.RenderHeal(g_screen);
		float player_x, player_y;
		p_player.GetPosition(player_x, player_y);					//lấy vị trí của người chơi

		//spawn quái
		if (enemy_list.size() < 25)
		{
			spawn_count = std::min(static_cast<int>(25 - enemy_list.size()), 5);
			if (current_time - lastSlimeSpawnTime >= spawnInterval) {
				slime->UpdateSpawnEnemies(enemy_list, g_screen, lastSlimeSpawnTime, spawnInterval, lastTime, p_player.GetCamera(), spawn_count, 0);
				lastSlimeSpawnTime = current_time;
			}

			if (current_time > lastTime + 180000) {  // 180000 ms = 3 phút
				if (current_time - lastBatSpawnTime >= spawnInterval * 2) {
					bat->UpdateSpawnEnemies(enemy_list, g_screen, lastBatSpawnTime, spawnInterval * 2, lastTime, p_player.GetCamera(), spawn_count, 1);
					lastBatSpawnTime = current_time;
				}
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

				SDL_Rect rect_player = p_player.GetRectFrame();
				SDL_Rect rect_enemy = p_enemy->GetRectFrame();
				bool bCol1 = SDLCommonFunc::CheckCollision(rect_player, rect_enemy);
				int enemy_dmg = p_enemy->GetDamage();
				if (bCol1)
				{
					Uint32 now = SDL_GetTicks();
					if (now - p_player.GetlastHitTime() >= p_player.GetInvincibleTime())
					{
						p_player.TakeDamage(enemy_dmg);
						p_player.RenderHeal(g_screen);
						p_player.SetlastHitTime(now);
						if (p_player.IsDead())
						{
							p_enemy->Free();
							Close();
							SDL_Quit();
							return 0;
						}
					}
				}
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
						SDL_Rect eRect = obj_enemy->GetRectFrame();

						SDL_Rect bRect = p_bullet->GetRect();

						bool bCol = SDLCommonFunc::CheckCollision(bRect, eRect);

						if (bCol)
						{
							p_player.RemoveBullet(bl);
							obj_enemy->TakeDamage(playerdamage);
							if (obj_enemy->IsDead())
							{
								kill_enemy++;
								obj_enemy->Free();
								delete obj_enemy;
								enemy_list.erase(enemy_list.begin() + i);
							}
						}
					}
				}
			}
		}
		RenderUI(g_screen, start_time, kill_enemy);
		
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
	if (TTF_Init() == -1)
	{
		std::cerr << "SDL_ttf could not initialize! TTF Error: " << TTF_GetError() << std::endl;
		return false;
	}
	font = TTF_OpenFont("assets//fonts//HARNGTON.ttf", 24);
	if (!font)
	{
		std::cerr << "font is error: " << TTF_GetError() << std::endl;
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

	TTF_CloseFont(font);

	SDL_DestroyRenderer(g_screen); // giải phóng renderer
	g_screen = NULL; // gán NULL cho renderer
	SDL_DestroyWindow(g_window); // giải phóng cửa sổ
	g_window = NULL; // gán NULL cho cửa sổ
	TTF_Quit();	//quit text
	IMG_Quit(); // quit ảnh
	SDL_Quit(); // quit SDL
}

void RenderUI(SDL_Renderer* renderer, Uint32 start_time, int kill_count)
{
	TTF_Font* font_text = font;
	Uint32 elapsed_time = SDL_GetTicks() - start_time;
	int seconds = elapsed_time / 1000;
	int minutes = seconds / 60;
	seconds %= 60;

	std::string time_str =  (minutes < 10 ? std::string("0") : "") + std::to_string(minutes) + ":" +
							(seconds < 10 ? std::string("0") : "") + std::to_string(seconds);
	std::string kill_str = std::to_string(kill_count);

	SDL_Color textColor = { 255, 255, 255 };

	if (font == nullptr) {
		std::cerr << "Font is NULL! TTF_Error: " << TTF_GetError() << std::endl;
	}

	SDL_Surface* timeSurface = TTF_RenderText_Solid(font_text, time_str.c_str(), textColor);
	if(timeSurface == nullptr)
	{
		std::cout << "loi load timeSurface" << TTF_GetError();
	}
	SDL_Texture* timeTexture = SDL_CreateTextureFromSurface(renderer, timeSurface);
	SDL_Rect timeRect = { SCREEN_WIDTH / 2, 20, timeSurface->w, timeSurface->h };
	SDL_RenderCopy(renderer, timeTexture, NULL, &timeRect);
	SDL_FreeSurface(timeSurface);
	SDL_DestroyTexture(timeTexture);

	SDL_Surface* killSurface = TTF_RenderText_Solid(font_text, kill_str.c_str(), textColor);
	SDL_Texture* killTexture = SDL_CreateTextureFromSurface(renderer, killSurface);
	SDL_Rect killRect = { 20, 20, killSurface->w, killSurface->h };
	SDL_RenderCopy(renderer, killTexture, NULL, &killRect);
	SDL_FreeSurface(killSurface);
	SDL_DestroyTexture(killTexture);
}
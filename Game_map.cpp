#include"stdafx.h"
#include"Game_map.h"
#include"CommonFunc.h"

void GameMap::LoadMap(const char* name) // hàm load map
{
	FILE* fp = NULL; // con trỏ file
	fopen_s(&fp, name, "rb"); // mở file ở chế độ đọc nhị phân
	if (fp == NULL) // nếu không mở được file
	{
		return;
	}

	game_map_.max_x_ = 0;	//gán 0 cho map_x	
	game_map_.max_y_ = 0;	// gán 0 cho map_y	

	for (int i = 0; i < MAX_MAP_Y; i++) // duyệt qua số ô theo chiều dọc
	{
		for (int j = 0; j < MAX_MAP_X; j++) // duyệt qua số ô theo chiều ngang
		{
			fscanf_s(fp, "%d", &game_map_.tile[i][j]); // đọc thông tin từ file
			int val = game_map_.tile[i][j]; // lưu thông tin vào biến val	
			if (val > 0)
			{
				if (j > game_map_.max_x_) // nếu j lớn hơn max_x
				{
					game_map_.max_x_ = j; // gán j cho max_x
				}
				if (i > game_map_.max_y_) // nếu i lớn hơn max_y
				{
					game_map_.max_y_ = i; // gán i cho max_y
				}
			}
		}
	}
	game_map_.max_x_ = (game_map_.max_x_ + 1) * TILE_SIZE; // tính max_x
	game_map_.max_y_ = (game_map_.max_y_ + 1) * TILE_SIZE; // tính max_y
	
	game_map_.start_x_ = 0; // gán 0 cho start_x
	game_map_.start_y_ = 0; // gán 0 cho start_y

	game_map_.file_name_ = name; // gán tên file cho file_name

	fclose(fp); // đóng file
}




void GameMap::LoadTiles(SDL_Renderer* screen) // hàm load tiles
{
	char file_img[30]; // mảng lưu trữ tên file ảnh
	FILE* fp = NULL; // con trỏ file

	for (int i = 0; i < MAX_TILES; i++) // duyệt qua số tiles
	{
		sprintf_s(file_img, "map//%d.png", i); // gán tên file ảnh cho file_img
		fopen_s(&fp, file_img, "rb"); // mở file ở chế độ đọc nhị phân
		if (fp == NULL)
		{
			continue;
		}
		fclose(fp); // đóng file
		tile_mat[i].LoadImg(file_img, screen); // load ảnh

	}
	
}

void GameMap::DrawMap(SDL_Renderer* screen, const SDL_Rect& camera)
{
	int start_x = camera.x / TILE_SIZE;
	int start_y = camera.y / TILE_SIZE;

	int end_x = (camera.x + SCREEN_WIDTH) / TILE_SIZE + 1;
	int end_y = (camera.y + SCREEN_HEIGHT) / TILE_SIZE + 1;

	for (int i = start_y; i < end_y; i++)
	{
		for (int j = start_x; j < end_x; j++)
		{
			if (i >= 0 && i < MAX_MAP_Y && j >= 0 && j < MAX_MAP_X)
			{
				int val = game_map_.tile[i][j];
				if (val > 0)
				{
					int render_x = j * TILE_SIZE - camera.x;
					int render_y = i * TILE_SIZE - camera.y;

					tile_mat[val].SetRect(render_x, render_y);
					tile_mat[val].Render(screen);
				}
			}
		}
	}
}


/*void GameMap::DrawMap(SDL_Renderer* screen, SDL_Rect& camera)
{
	int x1 = 0;
	int x2 = 0;

	int y1 = 0;
	int y2 = 0;

	int map_x = 0;
	int map_y = 0;

	map_x = game_map_.start_x_ / TILE_SIZE;

	x1 = (game_map_.start_x_ % TILE_SIZE) * -1; // tính x1
	x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE); // tính x2	

	map_y = game_map_.start_y_ / TILE_SIZE;

	y1 = (game_map_.start_y_ % TILE_SIZE) * -1; // tính y1	
	y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE); // tính y2

	for (int i = y1; i < y2; i += TILE_SIZE)			// duyệt qua số ô theo chiều dọc
	{
		map_x = game_map_.start_x_ / TILE_SIZE;
		for (int j = x1; j < x2; j += TILE_SIZE)
		{
			int val = game_map_.tile[map_y][map_x];
			if (val > 0)
			{
				tile_mat[val].SetRect(j, i);
				tile_mat[val].Render(screen);
			}
			map_x++;
		}
		map_y++;
	}
}*/

#ifndef GAME_MAP_H_
#define GAME_MAP_H_

#include"CommonFunc.h"
#include"BaseObject.h"

#define MAX_TILES 20   //	số lượng tiles	
 
class TileMat : public BaseObject
{
public:
	TileMat() { ; }
	~TileMat() { ; }	

};

class GameMap
{
public:
	GameMap() { ; }
	~GameMap() { ; }
	void LoadMap(const char* name);  // hàm load map	
	void LoadTiles(SDL_Renderer* screen);  // hàm load tiles	
	void DrawMap(SDL_Renderer* screen);  // hàm vẽ map	
	Map game_map_;	 // biến lưu trữ thông tin map
	TileMat tile_mat[MAX_TILES];	// mảng lưu trữ các tiles


};

#endif

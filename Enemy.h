
#ifndef ENEMY_H_
#define ENEMY_H_

#include"CommonFunc.h"
#include"BaseObject.h"

class Enemy : public BaseObject
{
public:
	Enemy();
	~Enemy();

private:		
	float speed_;											// tốc độ của quái
	float target_x, target_y;								//di chuyển đến chỗ người chơi

};

#endif // ENEMY_H_


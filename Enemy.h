
#ifndef ENEMY_H_
#define ENEMY_H_

#include"CommonFunc.h"
#include"BaseObject.h"

class Enemy : public BaseObject
{
public:
	Enemy(int frame_count);
	virtual ~Enemy();

	enum EnemyType
	{
		SLIME = 0,
		BAT = 1,
	};

	void set_enemy_x(const float enemy_x) { enemy_x_ = enemy_x; }
	void set_enemy_y(const float& enemy_y) { enemy_y_ = enemy_y; }
	float get_enemy_x() const { return enemy_x_; }
	float get_enemy_y() const { return enemy_y_; }

	void SetSpeed(float speed) { speed_ = speed; }
	void SetTarget(float target_x, float target_y) { target_x_ = target_x; target_y_ = target_y; }

	virtual void Move() = 0;
	void TakeDamage(int dmg);
	bool IsDead();
	void SetHp(int hp) { health_ = hp; }
	
	virtual bool LoadImgEnemy(std::string path, SDL_Renderer* screen) = 0;
	virtual void ShowEnemy(SDL_Renderer* des, const SDL_Rect& camera) = 0;	//	hàm hiển thị
	virtual void set_clipsEnemy() = 0;

	int get_width_frame() const { return width_frame_; }
	int get_height_frame() const { return height_frame_; }

	void SetType(const int type) { enemy_type_ = type;}
	virtual int GetType() const = 0;

	void SpawnEnemies(std::vector<Enemy*>& enemies, SDL_Renderer* renderer, int spawn_count, int enemy_type);
	void UpdateSpawnEnemies(std::vector<Enemy*>& enemies, SDL_Renderer* renderer, Uint32& last_spawn_time, Uint32 spawn_delay, Uint32 start_time, int spawn_count, int enemy_type);

private:	
	float enemy_x_, enemy_y_;								//vị trí của quái
	float target_x_, target_y_;								//chỉ định vào người chơi

	Uint32 enemy_lastFrameTime = 0; 
	const Uint32 enemy_frameDelay = 200;

	int enemy_type_;

protected:
	float speed_;											// tốc độ của quái
	int health_;											// máu của quái
	int frame_count_;										// frame của quái
	int width_frame_;
	int height_frame_;
	int frame_;
	SDL_Rect* frame_clip_;

};

#endif // ENEMY_H_


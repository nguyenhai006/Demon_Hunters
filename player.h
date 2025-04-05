
#ifndef PLAYER_H_
#define PLAYER_H_


#include"CommonFunc.h"
#include"BaseObject.h"
#include "Bullet.h"

#define PLAYER_SPEED 3

class player : public BaseObject
{
public:
	player();
	~player();

	enum WalkType {
		WALK_RIGHT = 10,
		WALK_LEFT = 11,
	};


	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);	//	hàm hiển thị	
	void HandleInputAction(SDL_Event events, SDL_Renderer* screen);	//	hàm xử lý sự kiện
	void set_clips();
	
	void DoPlayer();
	void UpdateCamera();
	SDL_Rect GetCamera() const { return camera; }
	
	void set_bullet_list(std::vector<Bullet*> bullet_list)
	{
		p_bullet_list_ = bullet_list;
	}
	std::vector<Bullet*> get_bullet_list() const { return p_bullet_list_; };

	void HandleBullet(SDL_Renderer* des);

	void GetPosition(float& x, float& y) { x = x_pos_; y = y_pos_; }

	void RemoveBullet(const int& ind);

private:

	std::vector<Bullet*> p_bullet_list_;		//mảng lưu trữ đạn

	float x_pos_;
	float y_pos_;

	int width_frame_;
	int height_frame_;

	static SDL_Rect camera;

	Uint32 player_lastFrameTime = 0;
	const Uint32 player_frameDelay = 300;

	SDL_Rect frame_clip_[animIndex];
	Input input_type_;
	int frame_;
	int status_;

	float angle_;   //góc đạn bắn

	int health_player;			//máu người chơi

	int player_dmg;

};


#endif // !PLAYER_H_
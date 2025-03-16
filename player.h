
#ifndef PLAYER_H_
#define PLAYER_H_

#include"CommonFunc.h"
#include"BaseObject.h"

#define PLAYER_SPEED 3

class player : public BaseObject
{
public:
	player();
	~player();

	enum WalkType {
		WALK_RIGHT = 0,
		WALK_LEFT = 1,
	};

	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);	//	hàm hiển thị	
	void HandleInputAction(SDL_Event events, SDL_Renderer* screen);	//	hàm xử lý sự kiện
	void set_clips();
	
	void DoPlayer();

private:

	float x_pos_;
	float y_pos_;

	int width_frame_;
	int height_frame_;

	SDL_Rect frame_clip_[8];
	Input input_type_;
	int frame_;
	int status_;


};


#endif // !PLAYER_H_
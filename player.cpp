
#include "stdafx.h"
#include"player.h"
#include"CommonFunc.h"

#define BULLET_SPAWN_OFFSET 0.6



player::player()
{
	frame_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	width_frame_ = 0;
	height_frame_ = 0;
	status_ = -1;
	input_type_ = { 0 };
	for (int i = 0; i < animIndex; i++)
	{
		frame_clip_[i] = { 0,0,0,0 };
	}
}

player::~player()
{

}

bool player::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);

	if (ret)
	{
		width_frame_ = rect_.w / animIndex;
		height_frame_ = rect_.h;
	}
	return ret;
}

void player::set_clips()
{
	if (width_frame_ > 0 && height_frame_ > 0)
	{

		for (int i = 0; i < animIndex; i++)
		{
			frame_clip_[i].x = i * width_frame_;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = width_frame_;
			frame_clip_[i].h = height_frame_;
		}
	}
}

void player::Show(SDL_Renderer* des)
{
	if (status_ == WALK_LEFT)
	{
		LoadImg("img//walk_left.png", des);
	}
	else if (status_ == WALK_RIGHT)
	{
		LoadImg("img//walk_right.png", des);
	}

	if (input_type_.left_ == 1 || input_type_.right_ == 1)
	{
		if (status_ != WALK_LEFT)
		{
			status_ = WALK_RIGHT;
		}	
	}

	if (input_type_.left_ == 1 || input_type_.right_ == 1 || input_type_.up_ == 1 || input_type_.down_ == 1)
	{
		frame_++;
	}
	else
	{
		frame_ = 0;
	}

	if (frame_ >= animIndex)
	{
		frame_ = 0;
	}

	rect_.x = static_cast<int>(round(x_pos_ - camera.x));
	rect_.y = static_cast<int>(round(y_pos_ - camera.y));

	SDL_Rect* current_clip = &frame_clip_[frame_];

	SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };

	SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);

}

void player::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
	if (events.type == SDL_KEYDOWN)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			status_ = WALK_RIGHT;
			input_type_.right_ = 1;
			input_type_.left_ = 0;

		}
		break;
		case SDLK_LEFT:
		{
			status_ = WALK_LEFT;
			input_type_.left_ = 1;
			input_type_.right_ = 0;
		}
		break;
		case SDLK_UP:
		{
			
			input_type_.up_ = 1;
			input_type_.down_ = 0;
		}
		break;
		case SDLK_DOWN:
		{
			input_type_.down_ = 1;
			input_type_.up_ = 0;
		}
		break;
		
		}
	} 
	else if (events.type == SDL_KEYUP)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			status_ = WALK_RIGHT;
			input_type_.right_ = 0;
		}
		break;
		case SDLK_LEFT:
		{
			status_ = WALK_LEFT;
			input_type_.left_ = 0;
		}
		break;
		case SDLK_UP:
		{
			input_type_.up_ = 0;
		}
		break;
		case SDLK_DOWN:
		{
			input_type_.down_ = 0;
		}
		break;
		case SDLK_1:
			Bullet* p_bullet = new Bullet();
			p_bullet->LoadImg("img//bullet.png", screen);

			p_bullet->SetRect(this->rect_.x + width_frame_ / 2, rect_.y + height_frame_ * BULLET_SPAWN_OFFSET);

			float bullet_angle = angle_;
			float speed = BULLET_SPEED;

			float velX = cos(bullet_angle) * speed;
			float velY = sin(bullet_angle) * speed;

			p_bullet->set_x_val(velX); \
			p_bullet->set_y_val(velY);
			p_bullet->set_angle(bullet_angle);

			p_bullet->set_is_move(true);

			p_bullet_list_.push_back(p_bullet);
		}
	}
	
}

SDL_Rect player::camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };


void player::DoPlayer()
{
	float dirX = 0.0f;
	float dirY = 0.0f;
	if (input_type_.up_ == 1) dirY -= 1.0f;
	if (input_type_.down_ == 1) dirY += 1.0f;
	if (input_type_.right_ == 1) dirX += 1.0f;
	if (input_type_.left_ == 1) dirX -= 1.0f;

	float length = sqrt(dirX * dirX + dirY * dirY);

	if (length > 0)
	{
		dirX /= length;
		dirY /= length;
		angle_ = atan2(dirY, dirX);
	}

	x_pos_ += dirX * PLAYER_SPEED;
	y_pos_ += dirY * PLAYER_SPEED;


	// giới hạn nhận vật trong map
	if (x_pos_ < 0) x_pos_ = 0;
	if (y_pos_ < 0) y_pos_ = 0;
	if (x_pos_ > MAX_X) x_pos_ = MAX_X - width_frame_; 
	if (y_pos_ > MAX_Y) y_pos_ = MAX_Y - height_frame_;
	UpdateCamera();

}

void player::UpdateCamera()
{
	camera.x = static_cast<int>(x_pos_ + width_frame_ / 2 - SCREEN_WIDTH / 2);
	camera.y = static_cast<int>(y_pos_ + height_frame_ / 2 - SCREEN_HEIGHT / 2);

	// Giới hạn camera
	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;
	if (camera.x > MAX_X - SCREEN_WIDTH) camera.x = MAX_X - SCREEN_WIDTH;
	if (camera.y > MAX_Y - SCREEN_HEIGHT) camera.y = MAX_Y - SCREEN_HEIGHT;
}

void player::HandleBullet(SDL_Renderer* des)
{
	for (int i = 0; i < p_bullet_list_.size(); i++)
	{
		Bullet* p_bullet = p_bullet_list_.at(i);
		if (p_bullet != NULL)
		{
			if (!p_bullet->get_texture()) {		// load lại ảnh nếu chưa được
				p_bullet->LoadImg("img//bullet.png", des);
			}
			if (p_bullet->get_is_move() == true)
			{
				p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
				p_bullet->Render(des);
			}
			else
			{
				p_bullet_list_.erase(p_bullet_list_.begin() + i);
				if (p_bullet != NULL)
				{
					delete p_bullet;
					p_bullet = NULL;
				}
			}
		}
	}
}


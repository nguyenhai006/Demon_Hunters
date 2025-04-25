
#include "stdafx.h"
#include"player.h"
#include"CommonFunc.h"

#define BULLET_SPAWN_OFFSET 0.5



player::player()
{
	frame_ = 0;
	//x_pos_ = 0;
	//y_pos_ = 0;
	width_frame_ = 0;
	height_frame_ = 0;
	status_ = -1;
	input_type_ = { 0 };
	for (int i = 0; i < animIndex; i++)
	{
		frame_clip_[i] = { 0,0,0,0 };
	}
	player_dmg = 30;
	health_player = 50;
	current_health = health_player;
}

player::~player()
{

}

SDL_Rect player::camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

//Load ảnh nhân vật
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

//lấy kích thước ảnh
SDL_Rect player::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = width_frame_;
	rect.h = height_frame_;
	return rect;
}

//Thiết lập frame cho hoạt ảnh
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


//Show nhân vật lên màn hình
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

	Uint32 current_time = SDL_GetTicks();

	if (current_time > player_lastFrameTime + player_frameDelay)
	{
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
	}

	rect_.x = x_pos_ - camera.x;
	rect_.y = y_pos_ - camera.y;

	SDL_Rect* current_clip = &frame_clip_[frame_];
	
	SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };

	SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);

}

//xác định event từ bàn phím
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
			Mix_Chunk* magic_shoot = Mix_LoadWAV("assets//Audio//shoot.mp3");
			if (!magic_shoot)
			{
				std::cout << "Failed to load shoot music! SDL_mixer Error: " << Mix_GetError() << std::endl;
				return;
			}
			Mix_PlayChannel(-1, magic_shoot, 0);
			Bullet* p_bullet = new Bullet();
			p_bullet->LoadImg("img//bullet.png", screen);

			p_bullet->SetRect(this->rect_.x + width_frame_ / 2, rect_.y + height_frame_ * BULLET_SPAWN_OFFSET);

			float bullet_angle = angle_;
			float speed = BULLET_SPEED;

			float velX = cos(bullet_angle) * speed;
			float velY = sin(bullet_angle) * speed;

			p_bullet->set_x_val(velX); 
			p_bullet->set_y_val(velY);
			p_bullet->set_angle(bullet_angle);

			p_bullet->set_is_move(true);

			p_bullet_list_.push_back(p_bullet);
		}
	}
	
}

//di chuyển nhân vật
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
	if (x_pos_ + width_frame_ > MAX_X) x_pos_ = MAX_X - width_frame_; 
	if (y_pos_ + height_frame_ > MAX_Y) y_pos_ = MAX_Y - height_frame_;
	UpdateCamera();

}

//gắn camera theo nhân vật
void player::UpdateCamera()
{
	camera.x = x_pos_ + width_frame_ / 2 - SCREEN_WIDTH / 2;
	camera.y = y_pos_ + height_frame_ / 2 - SCREEN_HEIGHT / 2;

	// Giới hạn camera
	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;
	if (camera.x > MAX_X - SCREEN_WIDTH) camera.x = MAX_X - SCREEN_WIDTH;
	if (camera.y > MAX_Y - SCREEN_HEIGHT) camera.y = MAX_Y - SCREEN_HEIGHT;
}

//Băn đạn
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

//Xóa đạn
void player::RemoveBullet(const int& ind)
{
	if (p_bullet_list_.size() > 0 && p_bullet_list_.size() > ind)
	{
		Bullet* p_bullet = p_bullet_list_.at(ind);
		p_bullet_list_.erase(p_bullet_list_.begin() + ind);

		if (p_bullet)
		{
			delete p_bullet;
			p_bullet = NULL;
		}
	}
}

//nhân vật nhận damage
void player::TakeDamage(int dmg)
{
	current_health -= dmg;
}

//kiểm tra xem nhận vật còn sống không
bool player::IsDead()
{
	return current_health <= 0;
}

void player::reset()
{
	current_health = health_player;
	p_bullet_list_.clear();
}

void player::RenderHeal(SDL_Renderer* render)
{
	int heal_x = SCREEN_WIDTH - 220;
	int heal_y = 20;

	int heal_width = 200;
	int heal_height = 20;

	SDL_Rect heal_rect = { heal_x, heal_y, heal_width, heal_height };
	SDL_SetRenderDrawColor(render, 167, 175, 180, 255);
	SDL_RenderFillRect(render, &heal_rect);

	float hp_percent = static_cast<float> (current_health) / health_player;

	SDL_Rect fill_health = { heal_x, heal_y, static_cast<int>(hp_percent * heal_width), heal_height };
	SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
	SDL_RenderFillRect(render, &fill_health);
}
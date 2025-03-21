
#include"stdafx.h"
#include"Bullet.h"

Bullet::Bullet()
{
	x_val_ = 0;
	y_val_ = 0;
	is_move_ = false;
}

Bullet::~Bullet()
{

}

void Bullet::HandleMove(const int x_boder, const int y_boder)
{
	rect_.x += x_val_;
	rect_.y += y_val_;
	if (rect_.x < 0 || rect_.x > x_boder ||
		rect_.y < 0 || rect_.y > y_boder)
	{
		is_move_ = false;
	}
}
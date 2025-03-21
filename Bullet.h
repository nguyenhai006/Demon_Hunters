
#ifndef BULLET_H_
#define BULLET_H_

#include"CommonFunc.h"
#include"BaseObject.h"

#define BULLET_SPEED 8

class Bullet : public  BaseObject
{
public:
	Bullet();
	~Bullet();

		
	void set_x_val(const int& xVal) { x_val_ = xVal; }												// gán giá trị vận tốc đạn
	void set_y_val(const int& yVal) { y_val_ = yVal; }					
	int get_x_val() const { return x_val_; }														// thiết lập giá trị vận tốc
	int get_y_val() const { return y_val_; }

	void set_is_move(const bool& isMove) { is_move_ = isMove; }										// xác định đạn có di chuyển không
	bool get_is_move() const { return is_move_; }													// kiểm tra trạng thái di chuyển

	void set_angle(float angle) { angle_ = angle; }													//gắn góc đạn bắn
	float get_angle() const { return angle_; }														//thiết lập góc bắn

	SDL_Texture* get_texture() const { return p_object_; }											// hiển thị hình ảnh đạn

	void HandleMove(const int x_boder, const int y_boder);														// xử lý đạn di chuyển

private:
	int x_val_;
	int y_val_;
	bool is_move_;
	unsigned int bullet_dir_;
	float angle_;		//góc đạn bắn
};

#endif 

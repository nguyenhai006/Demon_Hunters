#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include"CommonFunc.h"

class BaseObject
{
public:
	BaseObject();
	~BaseObject();
	void SetRect(const int& x, const int& y) { rect_.x = x, rect_.y = y; }  // hàm set vị trí cho ảnh
	SDL_Rect GetRect() const { return rect_; }  // hàm lấy kích thước ảnh
	SDL_Texture* GetObject() const { return p_object_; }	// hàm lấy ảnh
	
	bool LoadImg(std::string path, SDL_Renderer* screen);  // hàm load ảnh	
	void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);   // hàm render ảnh
	void Free();  // hàm giải phóng ảnh

protected:	
	SDL_Texture* p_object_;  // biến lưu trữ ảnh
	SDL_Rect rect_;  // biến lưu trữ kích thước ảnh
};
		


#endif

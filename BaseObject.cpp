#include"stdafx.h"
#include"BaseObject.h"

BaseObject::BaseObject()
{
	p_object_ = NULL;
	rect_.x = 0;
	rect_.y = 0;
	rect_.w = 0;
	rect_.h = 0;
}

BaseObject::~BaseObject()
{
	Free();
}

bool BaseObject::LoadImg(std::string path, SDL_Renderer* screen) // hàm load ảnh
{
	Free(); // giải phóng ảnh cũ
	SDL_Texture* new_texture = NULL; // biến lưu trữ ảnh mới
	SDL_Surface* load_surface = IMG_Load(path.c_str()); // load ảnh từ đường dẫn
	if (load_surface != NULL) // nếu ảnh được load thành công
	{
		SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B)); // set màu trong suốt cho ảnh
		new_texture = SDL_CreateTextureFromSurface(screen, load_surface);		//tạo ảnh mới từ ảnh load được
		if (new_texture != NULL) // nếu ảnh mới được tạo thành công
		{	
			rect_.w = load_surface->w; // lấy chiều rộng của ảnh
			rect_.h = load_surface->h; // lấy chiều cao của ảnh
		}
		SDL_FreeSurface(load_surface);	// giải phóng bộ nhớ
	}
	p_object_ = new_texture;	// gán ảnh mới cho biến lưu trữ ảnh
	return p_object_ != NULL;	// trả về true nếu ảnh được load thành công
}

void BaseObject::Render(SDL_Renderer* des, const SDL_Rect* clip) // hàm render ảnh
{
	SDL_Rect renderquad = { rect_.x, rect_.y, rect_.w, rect_.h }; // tạo một hình chữ nhật render
	if (clip != NULL) // nếu clip khác NULL
	{
		renderquad.w = clip->w; // lấy chiều rộng của clip
		renderquad.h = clip->h; // lấy chiều cao của clip
	}
	SDL_RenderCopy(des, p_object_, clip, &renderquad);
}	

void BaseObject::Free() // hàm giải phóng ảnh
{
	if (p_object_ != NULL) // nếu ảnh khác NULL
	{
		SDL_DestroyTexture(p_object_); // giải phóng ảnh
		p_object_ = NULL; // gán NULL cho biến lưu trữ ảnh
		rect_.w = 0; // gán 0 cho chiều rộng
		rect_.h = 0; // gán 0 cho chiều cao
	}
}
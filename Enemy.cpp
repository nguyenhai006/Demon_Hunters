
#include"stdafx.h"
#include"Enemy.h"
#include"SlimeAndBat.h"
#include <algorithm>

Enemy::Enemy(int frame_count) : frame_(0), health_(10), speed_(1.0f), frame_count_(frame_count) {
    enemy_x_ = 0;
    enemy_y_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    enemy_type_ = -1;
    frame_clip_ = new SDL_Rect[frame_count_];
}

Enemy::~Enemy() {
    if (frame_clip_ != nullptr) {
        delete[] frame_clip_; 
        frame_clip_ = nullptr; 
    }
}

//Hàm di chuyển của quái
void Enemy::Move()
{
    float dx = target_x_ - enemy_x_;
    float dy = target_y_ - enemy_y_;

    float distance = sqrt(dx * dx + dy * dy);

    if (distance > 0)
    {
        enemy_x_ += (dx / distance) * speed_;
        enemy_y_ += (dy / distance) * speed_;
    }

}


//Hàm lấy kích thước ảnh quái
SDL_Rect Enemy::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;
    return rect;
}


//Hàm load ảnh quái
bool Enemy::LoadImgEnemy(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);

    if (!ret) {
        std::cerr << "Lỗi load ảnh quái: " << path << std::endl;
        return false;
    }

    if (ret)
    {
        width_frame_ = rect_.w / frame_count_;
        height_frame_ = rect_.h;
    }
    return ret;
}

//Thiết lập frame cho hoạt ảnh
void Enemy::set_clipsEnemy()
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {

        for (int i = 0; i < frame_count_; i++)
        {
            frame_clip_[i].x = i * width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

//Vẽ quái lên màn hình
void Enemy::ShowEnemy(SDL_Renderer* des, const SDL_Rect& camera)
{
    rect_.x = enemy_x_ - camera.x;
    rect_.y = enemy_y_ - camera.y;

    Uint32 current_time = SDL_GetTicks();

    if (current_time > enemy_lastFrameTime + enemy_frameDelay) {
        frame_++;
        if (frame_ >= frame_count_) {
            frame_ = 0;
        }
        enemy_lastFrameTime = current_time;
    }

    if (frame_ >= frame_count_)
    {
        frame_ = 0;
    }

    SDL_Rect* current_clip = &frame_clip_[frame_];
    SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
    SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
}


//Hàm chọn vị trí spawn quái
void Enemy::SpawnEnemies(std::vector<Enemy*>& enemies, SDL_Renderer* renderer, const SDL_Rect& camera, int spawn_count, int enemy_type) {
    for (int i = 0; i < spawn_count; ++i) {
        Enemy* new_enemy = nullptr;

        if (enemy_type == 0) {
            new_enemy = new Slime(); // Tạo Slime
        }
        else {
            new_enemy = new Bat();   // Tạo Bat
        }
        int spawn_x = (rand() % (SCREEN_WIDTH - 100)) + 50 + camera.x;                 // quái spawn trong màn hình
        int spawn_y = (rand() % (SCREEN_HEIGHT - 100)) + 50 + camera.y;

        new_enemy->set_enemy_x(spawn_x);
        new_enemy->set_enemy_y(spawn_y);

        std::string enemy_img_path;
        if (enemy_type == 0) {
            enemy_img_path = "img//Slime.png";
        }
        else {
            enemy_img_path = "img//bat.png";
        }

        std::cout << i << ": "  << "Spawned enemy type: " << enemy_type << " at (" << spawn_x << ", " << spawn_y << ")" << std::endl;

        if (new_enemy->LoadImgEnemy(enemy_img_path, renderer)) {
            enemies.push_back(new_enemy);
        }
        else {
            delete new_enemy;
        }
    }
}

//Spawn quái
void Enemy::UpdateSpawnEnemies(std::vector<Enemy*>& enemies, SDL_Renderer* renderer, Uint32& last_spawn_time, Uint32 spawn_delay, Uint32 start_time, const SDL_Rect& camera, int spawn_count, int enemy_type) {
    Uint32 current_time = SDL_GetTicks();
    Uint32 elapsed_time = (current_time - start_time) / 1000;

    spawn_count = 5 + elapsed_time / 180000;            //cứ sau 3' thì tăng lượng quái spawn lên            
    if (current_time > last_spawn_time + spawn_delay) {
        SpawnEnemies(enemies, renderer,camera, spawn_count, enemy_type);
        last_spawn_time = current_time;
    }
}

//Hàm quái nhận damage 
void Enemy::TakeDamage(int dmg)
{
    health_ -= dmg;
}

//Kiểm tra xem quái chết chưa
bool Enemy::IsDead()
{
    return health_ <= 0;
}




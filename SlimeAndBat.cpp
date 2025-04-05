
#include"stdafx.h"
#include"SlimeAndBat.h"

Slime::Slime() : Enemy(8) {
    speed_ = 0.7f;
}

void Slime::Move() {
    Enemy::Move();
}

void Slime::set_clipsEnemy() {
    Enemy::set_clipsEnemy();
}

void Slime::ShowEnemy(SDL_Renderer* des, const SDL_Rect& camera) {
    Enemy::ShowEnemy(des,camera);
}

bool Slime::LoadImgEnemy(std::string path, SDL_Renderer* screen) {
    return Enemy::LoadImgEnemy("img//Slime.png", screen);
}

Bat::Bat() : Enemy(4) {
    speed_ = 1.0f;
}

void Bat::Move() {
    Enemy::Move();
}

void Bat::set_clipsEnemy() {
    Enemy::set_clipsEnemy();
}

void Bat::ShowEnemy(SDL_Renderer* des, const SDL_Rect& camera) {
    Enemy::ShowEnemy(des, camera);
}

bool Bat::LoadImgEnemy(std::string path, SDL_Renderer* screen) {
    return Enemy::LoadImgEnemy("img//bat.png", screen);
}
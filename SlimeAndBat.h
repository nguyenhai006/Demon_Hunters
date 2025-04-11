
#ifndef SLIME_AND_BAT_H_
#define SLIME_AND_BAT_H_

#include"CommonFunc.h"
#include"BaseObject.h"
#include"Enemy.h"

class Slime : public Enemy
{
public:
	Slime();
	void Move() override;
	void set_clipsEnemy() override;
	void ShowEnemy(SDL_Renderer* des, const SDL_Rect& camera) override;
	int GetType() const override { return 0; }
	bool LoadImgEnemy(std::string path, SDL_Renderer* screen) override;
	int GetDamage() const { return 10; }
};

class Bat : public Enemy
{
public:
	Bat();
	void Move() override;
	void set_clipsEnemy() override;
	void ShowEnemy(SDL_Renderer* des, const SDL_Rect& camera) override;
	int GetType() const override { return 1; }
	bool LoadImgEnemy(std::string path, SDL_Renderer* screen) override;
	int GetDamage() const { return 15; }
};

#endif


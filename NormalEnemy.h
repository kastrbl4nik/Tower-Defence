#pragma once
#include "Enemy.h"

class NormalEnemy : public Enemy {
public:
	NormalEnemy(COORD* path) : Enemy(path) {
		sprite		= { SPRITE::NORMAL_ENEMY, SPRITE::NORMAL_ENEMY_ATTR };
		speed		= 15;
		damage		= 1;
		reloadTime	= 0.5;
		health		= 10;
	}
};
#pragma once
#include "Enemy.h"

class FastEnemy : public Enemy {
public:
	explicit FastEnemy(COORD* path) : Enemy(path) {

		sprite		= { SPRITE::FAST_ENEMY, SPRITE::FAST_ENEMY_ATTR };
		speed		= 30;
		damage		= 1;
		health		= 7;

	}
};
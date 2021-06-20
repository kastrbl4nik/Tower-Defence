#pragma once
#include "Enemy.h"

class HeavyEnemy : public Enemy {
public:
	explicit HeavyEnemy(COORD* path) : Enemy(path) {

		sprite		= { SPRITE::HEAVY_ENEMY, SPRITE::HEAVY_ENEMY_ATTR };
		speed		= 8;
		damage		= 3;
		health		= 15;

	}
};
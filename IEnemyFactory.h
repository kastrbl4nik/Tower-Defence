#pragma once
#include "Enemy.h"

class IEnemyFactory {
public:
	virtual ~IEnemyFactory() {};
	virtual Enemy* create(COORD* path) const = 0;
};
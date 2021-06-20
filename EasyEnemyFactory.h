#pragma once
#include "IEnemyFactory.h"

#include "NormalEnemy.h"
#include "HeavyEnemy.h"
#include "FastEnemy.h"

class EasyEnemyFactory : public IEnemyFactory {
public:
	Enemy* create(COORD* path) const override {
		return new NormalEnemy(path);
	}
};
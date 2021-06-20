#pragma once
#include "IEnemyFactory.h"

#include "NormalEnemy.h"
#include "HeavyEnemy.h"
#include "FastEnemy.h"

class HardEnemyFactory : public IEnemyFactory {
public:
	Enemy* create(COORD* path) const override {
		switch (1 + (rand() % 3)) {
		case 1:
			return new NormalEnemy(path);
		case 2:
			return new HeavyEnemy(path);
		case 3:
			return new FastEnemy(path);
		}
	}
};
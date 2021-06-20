#pragma once
#include "IEnemyFactory.h"

#include "NormalEnemy.h"
#include "HeavyEnemy.h"
#include "FastEnemy.h"

class NormalEnemyFactory : public IEnemyFactory {
public:
	Enemy* create(COORD* path) const override {
		switch (1 + (rand() % 2)) {
		case 1:
			return new NormalEnemy(path);
		case 2:
			return new FastEnemy(path);
		}
	}
};
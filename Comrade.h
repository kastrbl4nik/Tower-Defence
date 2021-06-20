#pragma once
#include "PathFollower.h"

class Comrade : public PathFollower {
public:
	Comrade(COORD* path) : PathFollower(path) {
		type		= OBJECT::COMRADE;
		sprite		= { SPRITE::COMRADE, SPRITE::COMRADE_ATTR };
		speed		= 10;
		health		= 1;
	}
};
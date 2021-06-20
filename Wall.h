#pragma once
#include "Object.h"
class Wall : public Object {
public:
	Wall(int x, int y) : Object(x, y) {
		type = OBJECT::WALL;
		sprite = { SPRITE::WALL, SPRITE::WALL_ATTR };
	}
};
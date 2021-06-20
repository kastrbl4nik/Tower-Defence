#pragma once
#include "Object.h"

class Base : public Object {
public:
	Base(int x, int y) : Object(x, y) {

		type		= OBJECT::BASE;
		sprite		= { SPRITE::WALL, SPRITE::WALL_ATTR };
		health		= 3;

	}
	void addComrade() {
		health++;
	}
};
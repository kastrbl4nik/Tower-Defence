#pragma once
#include "Entity.h"

class Projectile : public Entity {
	int damage;
public:
	Projectile(int x, int y, DIRECTION facing) : Entity(x, y) {

		type			= OBJECT::PROJECTILE;
		this->facing	= facing;
		sprite			= { SPRITE::PROJECTILE, SPRITE::PROJECTILE_ATTR };
		speed			= 99;
		damage			= 1;
		health			= 1;

	}
	int getNextX() override {
		int x = coords.X;
		switch (facing) {
		case DIRECTION::WEST:
			x--;
			break;
		case DIRECTION::EAST:
			x++;
			break;
		}
		return x;
	}
	int getNextY() override {
		int y = coords.Y;
		switch (facing) {
		case DIRECTION::NORTH:
			y--;
			break;
		case DIRECTION::SOUTH:
			y++;
			break;
		}
		return y;
	}
	void hit(Object* obj) {
		obj->setHealth(obj->getHealth() - damage);
		die();
	}

	int getDamage() { return damage; }
};
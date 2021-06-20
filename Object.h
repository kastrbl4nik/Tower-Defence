#pragma once
#include <Windows.h>

enum class OBJECT { PLAYER, ENEMY, PROJECTILE, COMRADE, WALL, ROAD, BASE, UNKNOWN };

enum SPRITE {
	PLAYER = u'\u2665',
	PROJECTILE = u'\u25CA',
	FAST_ENEMY = u'\u25B2',
	NORMAL_ENEMY = u'\u2666',
	HEAVY_ENEMY = u'\u263B',
	COMRADE = u'\u263B',
	UNKNOWN = u'\u203C',
	PLAYER_ATTR = COLOUR::FG_YELLOW | COLOUR::BG_BLACK,
	PROJECTILE_ATTR = COLOUR::FG_CYAN | COLOUR::BG_BLACK,
	FAST_ENEMY_ATTR = COLOUR::FG_CYAN | COLOUR::BG_BLACK,
	NORMAL_ENEMY_ATTR = COLOUR::FG_RED | COLOUR::BG_BLACK,
	HEAVY_ENEMY_ATTR = COLOUR::FG_MAGENTA | COLOUR::BG_BLACK,
	COMRADE_ATTR = COLOUR::FG_GREEN | COLOUR::BG_BLACK,
	UNKNOWN_ATTR = COLOUR::FG_BLACK | COLOUR::BG_MAGENTA,

	AIR = u' ',
	ROAD = u'\u2591',
	WALL = u'\u2588',
	BASE = u' ',
	AIR_ATTR = COLOUR::BG_BLACK,
	WALL_ATTR = COLOUR::FG_DARK_MAGENTA,
	ROAD_ATTR = COLOUR::FG_DARK_GREY | COLOUR::BG_BLACK,
	BASE_ATTR = COLOUR::BG_GREEN,
	TOWER_ATTR = COLOUR::BG_GREEN
};

class Object {
protected:
	COORD coords;
	CHAR_INFO sprite;
	OBJECT type;
	int health;
public:
	Object(int x, int y) {
		coords	= { (SHORT)x, (SHORT)y };
		health	= 1;
		sprite	= { SPRITE::UNKNOWN, SPRITE::UNKNOWN_ATTR };
		type	= OBJECT::UNKNOWN;
	}
	virtual ~Object() {};
	int getX() {
		return coords.X;
	}
	int getY() {
		return coords.Y;
	}
	CHAR_INFO getSprite() {
		return sprite;
	}
	OBJECT getType() {
		return type;
	}
	void setCoords(int x, int y) {
		coords = { (SHORT)x, (SHORT)y };
	}
	bool isDead() {
		if (health <= 0)
			return 1;
		else
			return 0;
	}
	void die() {
		this->health = 0;
	}
	int getHealth() {
		return health;
	}
	void setHealth(int value) {
		health = value;
	}
};
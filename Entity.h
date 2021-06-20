#pragma once

#include <Windows.h>
#include <chrono>

#include "Map.h"
#include "Object.h"

enum class DIRECTION {
	NORTH,
	SOUTH,
	WEST,
	EAST,
	NONE
};

class Entity : public Object {
protected:
	DIRECTION	facing;
	int			speed;	//tiles per second

	std::chrono::system_clock::time_point lastMove;
public:
	Entity(int x, int y) : Object(x, y) {
		lastMove = std::chrono::system_clock::now();
	}

	virtual int getNextX() = 0;
	virtual int getNextY() = 0;

	virtual ~Entity() {};

	bool walk(Map* map) {
		int x = getNextX(),
			y = getNextY();

		if (isReadyToMove()) {
			if (map->isEmpty(x, y)) {
				map->erase(coords.X, coords.Y);
				coords = { (SHORT)x, (SHORT)y };
				map->add(this);
				lastMove = std::chrono::system_clock::now();
			}
			else
				return 0;
		}
		return 1;
	}

	bool isReadyToMove() {
		auto current = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime;
		elapsedTime = current - lastMove;
		if (elapsedTime.count() * speed > 1)
			return 1;
		else
			return 0;
	}

	void setLastMove(std::chrono::system_clock::time_point lastMove) {
		this->lastMove = lastMove;
	}

	void setFacing(DIRECTION facing) {
		this->facing = facing;
	}
};
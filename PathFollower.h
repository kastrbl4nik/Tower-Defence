#pragma once
#include "Entity.h"

class PathFollower : public Entity {
	COORD*	path;
	int		objective = 0;
public:
	PathFollower(COORD* path) : Entity(path[0].X, path[0].Y) {
		this->path	= path;
		objective	= 0;
	}
	int getNextX() override {
		int x = coords.X;
		if (path[objective].X == x && path[objective].Y == coords.Y) objective++;
		if (path[objective].X > x) x++;
		if (path[objective].X < x) x--;
		return x;
	}
	int getNextY() override {
		int y = coords.Y;
		if (path[objective].X == coords.X && path[objective].Y == y) objective++;
		if (path[objective].Y > y) y++;
		if (path[objective].Y < y) y--;
		return y;
	}
};
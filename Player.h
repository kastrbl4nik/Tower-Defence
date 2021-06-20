#pragma once
#include "Entity.h"
#include "Projectile.h"
#include "Tower.h"
#include "Wall.h"

class Player : public Entity {
private:

	COORD	nextCoords;
	int		ammo, coins;
	float	reloadTime, buildDelay;

	std::chrono::system_clock::time_point lastShot, lastBuild;

	int getBulletX() {
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
	int getBulletY() {
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

public:

	Player(int x, int y, DIRECTION facing) : Entity(x, y) {

		type			= OBJECT::PLAYER;
		this->facing	= facing;
		sprite			= { SPRITE::PLAYER, SPRITE::PLAYER_ATTR };
		speed			= 12;
		health			= 3;
		buildDelay		= 0.2;
		reloadTime		= 0.6;
		ammo			= 6;
		coins			= 9;
		lastShot		= std::chrono::system_clock::now();
		lastBuild		= std::chrono::system_clock::now();
		nextCoords		= { (SHORT)x, (SHORT)y };

	}

	int getNextX() override {
		return nextCoords.X;
	}
	int getNextY() override {
		return nextCoords.Y;
	}

	bool isReadyToShoot() {
		auto current = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime;
		elapsedTime = current - lastShot;
		if (elapsedTime.count() >= reloadTime && ammo > 0)
			return 1;
		else
			return 0;
	}

	bool isReadyToBuild() {
		auto current = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime;
		elapsedTime = current - lastBuild;
		if (elapsedTime.count() >= buildDelay)
			return 1;
		else
			return 0;
	}

	void shoot(Map* map, std::vector<Projectile*>& projectiles) {
		if (isReadyToShoot()) {
			int x = getBulletX(),
				y = getBulletY();
			if (map->isEmpty(x, y)) {
 				projectiles.push_back(new Projectile(x, y, facing));
				map->add(*projectiles.rbegin());
				lastShot = std::chrono::system_clock::now();
				ammo--;
				mciSendString(TEXT("play \"Sounds/laser.wav\""), NULL, 0, NULL);
			}
		}
	}

	void buildTower(Map* map, std::vector<Tower*>& towers) {
		int x = coords.X,
			y = coords.Y - 2;
		if (coins >= 3 && map->isEmpty(x - 3, y - 3, x + 3, y + 1)
			&& map->isEmpty(x - 3, y + 3, x + 3, y + 3)) {
			mciSendString(TEXT("play \"Sounds/tower_placed.wav\""), NULL, 0, NULL);
			towers.push_back(new Tower(x, y, map));
			map->add(towers.back());
			coins -= towers.back()->getPrice();
		}
	}

	void sellTower(Map* map) {
		int x = coords.X,
			y = coords.Y - 2;
		if (Tower* tow = dynamic_cast<Tower*>(map->getObject(x, y))) {
			tow->collapse(map);
			mciSendString(TEXT("play \"Sounds/boom.wav\""), NULL, 0, NULL);
			coins += tow->getLvl();
		}
	}

	void upgradeTower(Map* map) {
		int x = coords.X,
			y = coords.Y - 2;
		if (isReadyToBuild()) {
			if (Tower* tow = dynamic_cast<Tower*>(map->getObject(x, y))) {
				if (coins > tow->getLvl() + 1 && tow->upgrade(map)) {
					mciSendString(TEXT("play \"Sounds/lvl_up.wav\""), NULL, 0, NULL);
					coins -= tow->getLvl() + 1;
					lastBuild = std::chrono::system_clock::now();
				}
			}
		}
	}

	void setNextCoords(COORD nextCoords) {
		this->nextCoords = nextCoords;
	}
	int getCoins() {
		return coins;
	}
	void gainCoins(int value) {
		coins += value;
	}
	int getAmmo() {
		return ammo;
	}
	void gainAmmo(int value) {
		ammo += value;
	}
};
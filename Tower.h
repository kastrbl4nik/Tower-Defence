#pragma once
#include "Object.h"
#include "Map.h"
#include "Wall.h"

class Tower : public Object {
	int lvl, price;
	//Enemy* target; 
	//TODO: bling blong
	class Canon {
		COORD coords;
		float reloadTime;
		std::chrono::system_clock::time_point lastShot;
		bool isReadyToShoot() {
			auto current = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime;
			elapsedTime = current - lastShot;
			if (elapsedTime.count() >= reloadTime)
				return 1;
			else
				return 0;
		}
	public:
		Canon(int x, int y) {
			coords		= { (SHORT)x, (SHORT)y };
			lastShot	= std::chrono::system_clock::now();
			reloadTime	= 1.2;
		}
		void shoot(Map* map, std::vector<Projectile*>& projectiles) {
			int x = coords.X,
				y = coords.Y;
			if (isReadyToShoot()) {
				//target = map->getEnemy(x - 20, y - 20, x + 20, y - 20);
				//mciSendString(TEXT("play \"Sounds/canon.wav\""), NULL, 0, NULL);
				if (map->isEmpty(x, y - 3)) {
					projectiles.push_back(new Projectile(x, y - 3, DIRECTION::NORTH));
					map->add(*projectiles.rbegin());
				}
				if (map->isEmpty(x, y + 2)) {
					projectiles.push_back(new Projectile(x, y + 2, DIRECTION::SOUTH));
					map->add(*projectiles.rbegin());
				}
				if (map->isEmpty(x + 2, y)) {
					projectiles.push_back(new Projectile(x + 2, y, DIRECTION::EAST));
					map->add(*projectiles.rbegin());
				}
				if (map->isEmpty(x - 2, y)) {
					projectiles.push_back(new Projectile(x - 2, y, DIRECTION::WEST));
					map->add(*projectiles.rbegin());
				}
				lastShot = std::chrono::system_clock::now();
			}
		}
		void setReloadTime(float reloadTime) {
			this->reloadTime = reloadTime;
		}
	}* canon;
public:
	Tower(int x, int y, Map* map) : Object(x, y) {
		price = 3;
		lvl = 1;
		type = OBJECT::WALL;
		sprite = { SPRITE::AIR, SPRITE::AIR_ATTR };
		canon = new Canon(x, y);

		map->add(new Wall(x - 2, y - 2));
		map->add(new Wall(x - 0, y - 2));
		map->add(new Wall(x + 2, y - 2));

		map->add(new Wall(x - 1, y - 1));
		map->add(new Wall(x, y - 1));
		map->add(new Wall(x + 1, y - 1));

		map->add(new Wall(x - 1, y));

		map->add(new Wall(x + 1, y));

		map->add(new Wall(x - 1, y + 1));
		map->add(new Wall(x, y + 1));
		map->add(new Wall(x + 1, y + 1));

		map->add(new Wall(x - 1, y + 2));
		map->add(new Wall(x + 1, y + 2));
	}
	void shoot(Map* map, std::vector<Projectile*>& projectiles) {
		canon->shoot(map, projectiles);
	}
	void collapse(Map* map) {
		health = 0;
		int x = coords.X,
			y = coords.Y;

		map->erase(x, y);
		map->erase(x - 2, y - 2);
		map->erase(x - 0, y - 2);
		map->erase(x + 2, y - 2);
		map->erase(x - 1, y - 1);
		map->erase(x, y - 1);
		map->erase(x + 1, y - 1);
		map->erase(x - 1, y);
		map->erase(x + 1, y);
		map->erase(x - 1, y + 1);
		map->erase(x, y + 1);
		map->erase(x + 1, y + 1);
		map->erase(x - 1, y + 2);
		map->erase(x + 1, y + 2);
	}
	bool upgrade(Map* map) {
		CHAR_INFO c;
		switch (lvl) {
		case 1:
			lvl++;
			canon->setReloadTime(0.8);
			c = { SPRITE::WALL, COLOUR::FG_BLUE | COLOUR::BG_BLACK };
			break;
		case 2:
			lvl++;
			canon->setReloadTime(0.5);
			c = { SPRITE::WALL, COLOUR::FG_RED | COLOUR::BG_BLACK };
			break;
		case 3:
			lvl++;
			canon->setReloadTime(0.3);
			c = { SPRITE::WALL, COLOUR::FG_WHITE | COLOUR::BG_BLACK };
			break;
		default:
			return 0;
		}

		int x = coords.X,
			y = coords.Y;

		map->draw(x - 2, y - 2, c);
		map->draw(x - 0, y - 2, c);
		map->draw(x + 2, y - 2, c);
		map->draw(x - 1, y - 1, c);
		map->draw(x, y - 1, c);
		map->draw(x + 1, y - 1, c);
		map->draw(x - 1, y, c);
		map->draw(x + 1, y, c);
		map->draw(x - 1, y + 1, c);
		map->draw(x, y + 1, c);
		map->draw(x + 1, y + 1, c);
		map->draw(x - 1, y + 2, c);
		map->draw(x + 1, y + 2, c);
		return 1;
	}
	int getLvl() {
		return lvl;
	}
	int getPrice() {
		return price;
	}
};
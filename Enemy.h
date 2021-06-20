#pragma once
#include "PathFollower.h"

class Enemy : public PathFollower {
private:
	std::chrono::system_clock::time_point lastHit;
	bool isReadyToHit() {
		auto current = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime;
		elapsedTime = current - lastHit;
		if (elapsedTime.count() >= reloadTime)
			return 1;
		else
			return 0;
	}
public:
	Enemy(COORD* path) : PathFollower(path) {

		type		= OBJECT::ENEMY;
		lastHit		= std::chrono::system_clock::now();

	}
	void hit(Object* obj) {
		if (isReadyToHit()) {
			obj->setHealth(obj->getHealth() - damage);
			lastHit = std::chrono::system_clock::now();
		}
	}
protected:
	int damage;
	float reloadTime;
};
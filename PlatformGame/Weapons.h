#ifndef Weapons_h
#define Weapons_h

#include "Game.h"

class Weapons
{
public:
	Weapons(std::string name, int cnt, int bul, float wght, bool cs) : id(name), count(cnt), bullets(bul), weight(wght), canShoot(cs)
	{
	}
	~Weapons(){}

	void addCount() {
		count++;
	}
	bool shoot() {
		if (bullets > 0) {
			bullets--;
			return true;
		}
		return false;
	}

	bool throws(){
		if (count > 0) {
			count--;
			return true;
		}
	return false;
	}

private:
	std::string id;
	int count;
	int bullets;
	float weight;
	bool canShoot;
	bool active;
};
#endif
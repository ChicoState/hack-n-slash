//Scott Backer

//Long Sword Weapon Class H: Creates a long sword weapon for use with a player/AI

#include <allegro5\allegro.h>

#ifndef LONGSWORDWEAPON_H
#define LONGSWORDWEAPON_H

#include "Weapon.h"

class LongSwordWeapon : public Weapon
{
public:
	LongSwordWeapon(ALLEGRO_EVENT& InputAlEvent);
	void Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection);

private:

};

#endif
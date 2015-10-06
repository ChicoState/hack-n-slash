//Scott Backer

//Sword Weapon Class H: Creates a sword weapon for use with a player/AI

#include <allegro5\allegro.h>

#ifndef SWORDWEAPON_H
#define SWORDWEAPON_H

#include "Weapon.h"

class SwordWeapon : public Weapon
{
public:
	SwordWeapon(ALLEGRO_EVENT& InputAlEvent);

private:

};

#endif
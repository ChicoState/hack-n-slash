//Scott Backer

//Bow Weapon Class H: Creates a Bow weapon for use with a player/AI

#include <allegro5\allegro.h>

#ifndef BOWWEAPON_H
#define BOWWEAPON_H

#include "Weapon.h"
#include "BowWeaponTile.h"

class BowWeapon : public Weapon
{
public:
	BowWeapon(ALLEGRO_EVENT& InputAlEvent, ALLEGRO_BITMAP *SpriteImage); //!The constructor for the Bow weapon
	void BowWeapon::EventHandler(); //Handles events for the Bow weapon class
	void Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection); //Draws the weapon to the screen

private:
	BowWeaponTile m_BowWeaponTile;
};

#endif
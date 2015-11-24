//Scott Backer

//Sword Weapon Class H: Creates a sword weapon for use with a player/AI

#include <allegro5\allegro.h>

#ifndef SWORDWEAPON_H
#define SWORDWEAPON_H

#include "Weapon.h"
<<<<<<< HEAD
=======
#include "SwordWeaponTile.h"
>>>>>>> Scott

class SwordWeapon : public Weapon
{
public:
<<<<<<< HEAD
	SwordWeapon(ALLEGRO_EVENT& InputAlEvent);
	void Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection);

private:

=======
	SwordWeapon(ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent); //!The constructor for the sword weapon
	void SwordWeapon::EventHandler(); //Handles events for the sword weapon class
	void Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection); //Draws the weapon to the screen

private:
	SwordWeaponTile m_SwordWeaponTile; //sprite tile for the sword weapon
>>>>>>> Scott
};

#endif
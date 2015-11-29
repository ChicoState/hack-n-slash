//Scott Backer

//Sword Weapon Class H: Creates a sword weapon for use with a player/AI

#include <allegro5\allegro.h>

#ifndef SWORDWEAPON_H
#define SWORDWEAPON_H

#include "Weapon.h"
#include "SwordWeaponTile.h"

class SwordWeapon : public Weapon
{
public:
	SwordWeapon(ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent); //!The constructor for the sword weapon
	~SwordWeapon(); //Destructor for the sword weapon class
	void SwordWeapon::EventHandler(); //Handles events for the sword weapon class
	void Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection); //Draws the weapon to the screen

private:
	ALLEGRO_EVENT_SOURCE m_SwordActiveEventSource; //The event source for the active sword custom event

	SwordWeaponTile m_SwordWeaponTile; //sprite tile for the sword weapon

	bool m_OnActive = true; //true on the first iteration of m_IsActive in the event queue timer
};

#endif
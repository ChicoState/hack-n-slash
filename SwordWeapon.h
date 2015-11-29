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
<<<<<<< HEAD
	SwordWeapon(ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent); //!The constructor for the sword weapon
	~SwordWeapon(); //Destructor for the sword weapon class
=======
	SwordWeapon(ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent, ALLEGRO_BITMAP *SpriteImage); //!The constructor for the sword weapon
>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31
	void SwordWeapon::EventHandler(); //Handles events for the sword weapon class
	void Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection); //Draws the weapon to the screen

private:
<<<<<<< HEAD
	ALLEGRO_EVENT_SOURCE m_SwordActiveEventSource; //The event source for the active sword custom event

	SwordWeaponTile m_SwordWeaponTile; //sprite tile for the sword weapon

	bool m_OnActive = true; //true on the first iteration of m_IsActive in the event queue timer
=======
	SwordWeaponTile m_SwordWeaponTile;
>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31
};

#endif
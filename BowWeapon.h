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
<<<<<<< HEAD
	BowWeapon(ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent); //!The constructor for the Bow weapon
=======
	BowWeapon(ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent, ALLEGRO_BITMAP *SpriteImage); //!The constructor for the Bow weapon
>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31
	~BowWeapon(); //destructor for the weapon class
	void BowWeapon::EventHandler(); //Handles events for the Bow weapon class
	void Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection); //Draws the weapon to the screen

private:
<<<<<<< HEAD
	ALLEGRO_EVENT_SOURCE m_ProjectileEventSource; //The event source for the projectile custom event

	BowWeaponTile m_BowWeaponTile; //bow weapon tile for sprite of bow weapon
=======
	ALLEGRO_EVENT_SOURCE m_ProjectileEventSource;

	BowWeaponTile m_BowWeaponTile;
>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31

	bool m_OnActive; //true on the first iteration of m_IsActive in the event queue timer
};

#endif
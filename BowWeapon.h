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
	BowWeapon(ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent, ALLEGRO_BITMAP *SpriteImage); //!The constructor for the Bow weapon
	~BowWeapon(); //destructor for the weapon class
	void BowWeapon::EventHandler(); //Handles events for the Bow weapon class
	void Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection); //Draws the weapon to the screen

private:
	ALLEGRO_EVENT_SOURCE m_ProjectileEventSource;

	BowWeaponTile m_BowWeaponTile;

	bool m_OnActive; //true on the first iteration of m_IsActive in the event queue timer
};

#endif
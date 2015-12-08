//Scott Backer

//PlayerTile Class H: Handles the sprite control for the Bow weapon

#include <allegro5\allegro.h>

#ifndef BOWWEAPONTILE_H
#define BOWWEAPONTILE_H

#include "Sprite.h"

class BowWeaponTile : public Sprite
{
public:
	BowWeaponTile(int InputXPosition, int InputYPosition, int FrameWidth, int FrameHeight,
		bool Collidable, bool Animated, bool Continuous, bool Looped, int AnimFPS); //!Constructor for the playertile class
	int Event_Handler(); //!Overloaded event handler from Sprite class
	void Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection, bool IsWeaponActive); //!Overloaded draw function from Sprite class
	void DrawWeaponActive(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection, bool IsWeaponActive); //!Draws the weapon if it is active
	void DrawWeaponInactive(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection, bool IsWeaponActive); //Draws the weapon if it is inactive
	void DrawProjectile(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection); //Draws the weapon projectile

private:

};

#endif
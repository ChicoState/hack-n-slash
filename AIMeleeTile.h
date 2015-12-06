//
// File: AITile.h
// Author: James Beller (Original class written by Scott Backer)
// Group: Hack-'n-Slash
// Date: 12/6/2015
//
// Note: This header file and the source file, AIMeleeTile.cpp, are copied from Scott's
// SwordWeaponTile files with a couple minor changes.
//

#include <allegro5\allegro.h>

#ifndef AIMELEETILE_H
#define AIMELEETILE_H

#include "Sprite.h"

class AIMeleeTile : public Sprite
{
public:
	AIMeleeTile(int InputXPosition, int InputYPosition, int FrameWidth, int FrameHeight,
		bool Collidable, bool Animated, bool Continuous, bool Looped, int AnimFPS); //!Constructor for the playertile class
	int Event_Handler(); //!Overloaded event handler from Sprite class
	void Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection, bool IsWeaponActive); //!Overloaded draw function from Sprite class

private:

};

#endif
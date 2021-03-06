//
// File: AITile.h
// Author: James Beller (Original class written by Scott Backer)
// Group: Hack-'n-Slash
// Date: 12/4/2015
//
// Note: This header file and the source file, AITile.cpp, are copied from Scott's
// PlayerTile files with a couple minor changes.
//
#include <allegro5\allegro.h>

#ifndef AITILE_H
#define AITILE_H

#include "Sprite.h"

class AITile : public Sprite
{
public:
	AITile(ALLEGRO_BITMAP *image, int InputPlayerXPosition, int InputPlayerYPosition, int FrameWidth, int FrameHeight,
		bool Collidable, bool Animated, bool Continuous, bool Looped, int AnimFPS); //!Constructor for the playertile class
	int Event_Handler(); //!Overloaded event handler from Sprite class
	void Draw(int InputPlayerXPosition, int InputPlayerYPosition); //!Overloaded draw function from Sprite class

private:

};

#endif
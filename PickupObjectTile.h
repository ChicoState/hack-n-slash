//Scott Backer

//PickupObjectTile Class H: Pickup Object Tile class that contains the sprite for the pickup object

#include <allegro5\allegro.h>

#ifndef PICKUPOBJECTTILE_H
#define PICKUPOBJECTTILE_H

#include "Sprite.h"

class PickupObjectTile : public Sprite
{
public:
	PickupObjectTile(ALLEGRO_BITMAP *SpriteImage, int InputXPosition, int InputYPosition, int FrameWidth, int FrameHeight,
		bool Collidable, bool Animated, bool Continuous, bool Looped, int AnimFPS); //constructor for the pickupobjecttile class
	void Draw(int DrawXCoordinate, int DrawYCoordinate); //Draws the pickupobjecttile

private:

};

#endif
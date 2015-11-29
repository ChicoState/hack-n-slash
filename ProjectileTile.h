//Scott Backer

//ProjectileTile Class H: Handles the sprite control for the Projectile weapon

#include <allegro5\allegro.h>

#ifndef PROJECTILEWEAPONTILE_H
#define PROJECTILEWEAPONTILE_H

#include "Sprite.h"

class ProjectileTile : public Sprite
{
public:
	ProjectileTile(int InputXPosition, int InputYPosition, int FrameWidth, int FrameHeight,
		bool Collidable, bool Animated, bool Continuous, bool Looped, int AnimFPS); //!Constructor for the projectiletile class
	int Event_Handler(); //!Overloaded event handler from Sprite class
	void Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection); //!Overloaded draw function from Sprite class
	void DrawProjectile(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection); //Draws the projectile

private:

};

#endif
//Scott Backer

//PlayerTile Class H: Handles the sprite control for the player character

#include <allegro5\allegro.h>

#ifndef PLAYERTILE_H
#define PLAYERTILE_H

#include "Sprite.h"

class PlayerTile : public Sprite
{
public:
<<<<<<< HEAD
	PlayerTile(ALLEGRO_BITMAP *Image, int InputPlayerXPosition, int InputPlayerYPosition, int FrameWidth, int FrameHeight,
		bool Collidable, bool TriggerTile, bool Animated, bool Looped, int AnimFPS);
	void Draw(float InputPlayerXPosition, float InputPlayerYPosition);
=======
	PlayerTile(int InputPlayerXPosition, int InputPlayerYPosition, int FrameWidth, int FrameHeight,
		bool Collidable, bool Animated, bool Continuous, bool Looped, int AnimFPS); //!Constructor for the playertile class
	int Event_Handler(); //!Overloaded event handler from Sprite class
	void Draw(int InputPlayerXPosition, int InputPlayerYPosition); //!Overloaded draw function from Sprite class
>>>>>>> Scott

private:

};

#endif
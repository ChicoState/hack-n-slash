//Scott Backer

//PlayerTile Class H: Handles the sprite control for the player character

#include <allegro5\allegro.h>

#ifndef PLAYERTILE_H
#define PLAYERTILE_H

#include "Sprite.h"

class PlayerTile : public Sprite
{
public:
	PlayerTile(ALLEGRO_BITMAP *SpriteImage, int InputPlayerXPosition, int InputPlayerYPosition, int FrameWidth, int FrameHeight,
		bool Collidable, bool Animated, bool Continuous, bool Looped, int AnimFPS); //!Constructor for the playertile class
	int Event_Handler(); //!Overloaded event handler from Sprite class
	void Draw(int InputPlayerXPosition, int InputPlayerYPosition); //!Overloaded draw function from Sprite class

private:

};

#endif
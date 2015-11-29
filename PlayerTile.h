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
	PlayerTile(int InputPlayerXPosition, int InputPlayerYPosition, int FrameWidth, int FrameHeight,
		bool Collidable, bool Animated, bool Continuous, bool Looped, int AnimFPS); //!Constructor for the playertile class
	int Event_Handler(); //!Overloaded event handler from Sprite class
	void Draw(int InputPlayerXPosition, int InputPlayerYPosition, bool SwordActive, bool BowActive); //!Overloaded draw function from Sprite class
=======
	PlayerTile(ALLEGRO_BITMAP *SpriteImage, int InputPlayerXPosition, int InputPlayerYPosition, int FrameWidth, int FrameHeight,
		bool Collidable, bool Animated, bool Continuous, bool Looped, int AnimFPS); //!Constructor for the playertile class
	int Event_Handler(); //!Overloaded event handler from Sprite class
	void Draw(int InputPlayerXPosition, int InputPlayerYPosition); //!Overloaded draw function from Sprite class
>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31

private:
	Sprite m_SwordSprite; //sprite for the sword weapon and attack
	Sprite m_BowSprite; //the sprite for the bow weapon and attack
};

#endif
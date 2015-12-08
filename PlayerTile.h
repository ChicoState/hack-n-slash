//Scott Backer

//PlayerTile Class H: Handles the sprite control for the player character

#include <allegro5\allegro.h>

#ifndef PLAYERTILE_H
#define PLAYERTILE_H

#include "Sprite.h"

class PlayerTile : public Sprite
{
public:
	PlayerTile(int InputPlayerXPosition, int InputPlayerYPosition, int FrameWidth, int FrameHeight,
		bool Collidable, bool Animated, bool Continuous, bool Looped, int AnimFPS); //!Constructor for the playertile class
	int Event_Handler(); //!Overloaded event handler from Sprite class
	void Draw(int InputPlayerXPosition, int InputPlayerYPosition, bool SwordActive, bool BowActive, bool BowNewActivation); //!Overloaded draw function from Sprite class

private:
	Sprite m_SwordSprite; //sprite for the sword weapon and attack
	Sprite m_BowSprite; //the sprite for the bow weapon and attack

	bool m_BowDrawActive; //true if the bow is active in animation and should be drawn else false
};

#endif
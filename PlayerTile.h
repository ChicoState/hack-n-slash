//Scott Backer

//PlayerTile Class H: Handles the sprite control for the player character

#include <allegro5\allegro.h>

#ifndef PLAYERTILE_H
#define PLAYERTILE_H

#include "Sprite.h"

class PlayerTile : Sprite
{
public:
	PlayerTile(int InputPlayerXPosition, int InputPlayerYPosition, int FrameWidth, int FrameHeight,
		bool Collidable, bool TriggerTile, bool Animated, bool Looped, int AnimFPS);
	void Draw(float InputPlayerXPosition, float InputPlayerYPosition);
	void SetXPosition(int InputPlayerXPosition);
	void SetYPosition(int InputPlayerYPosition);

private:

	float m_XPosition;
	float m_YPosition;
};

#endif